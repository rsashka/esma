#pragma once
#ifndef INCLUDED_NEWLANG_OBJECT_
#define INCLUDED_NEWLANG_OBJECT_

#include <pch.h>

#include <types.h>
#include <variable.h>
#include <fraction.h>

namespace newlang {

    /*
     * Аргумент по умолчанию может быть литерал или выражение.
     * Если аргумент по умолчанию — это выражение, то оно вычисляется только один раз для всей программы при загрузке модуля.
     * Аргументы по умолчанию парсятся из токенов (создаются или вычисляются) при загрузке модуля, т.е. при создании
     * аругментов по умолчанию, а сами значения хранятся уже как объекты.
     * 
     * Аргументы в функции всегда имеют номер, который начинается с единицы в порядке определения в прототипе + некоторые могут иметь наименование.
     * Даже обязательные аргументы (которые не имеют значения по умолчанию в прототипе финкции), могут быть переданы по имени, 
     * а сами именованные аргументы могут быть указаны в произвольном порядке. Поэтому в реализации Args для обязательных 
     * аргументов так же хранится имя аргумента, но его значение отсутствует.
     * 
     * Так как позционные аргументы тоже могут передавать как именованные, то контроль и подстчет кол-ва 
     * не именованных аргументов при вызове фунции ничего не определяет. 
     * Следовательно, при вызове функции не именованные аргументы записываются по порядку, а именованные по имени.
     * Контроль передаваемых аргументов производится на наличие значения у каждого аргумента.
     * Если при определении функции после всех аргументов стоит многоточие, то разрешается добавлять 
     * новые аргументы по мимо тех, которые уже определены в прототипе функции.
     */

    ObjType DictionarySummaryType(const Obj *obj);
    std::vector<int64_t> TensorShapeFromDict(const Obj *obj);
    torch::Tensor ConvertToTensor(Obj *obj, at::ScalarType type = at::ScalarType::Undefined, bool reshape = true);

    at::TensorOptions ConvertToTensorOptions(const Obj *obj);
    at::DimnameList ConvertToDimnameList(const Obj *obj);
    bool ParsePrintfFormat(Obj *args, size_t start = 1);

    enum class ConcatMode : uint8_t {
        Error = 0,
        Append = 1,
        Discard = 2,
    };
    /* 
     * Для строк, словарей и классов (преобразование в одно измерение), тензоров (преобразование согласно ConcatMode)
     */
    int64_t ConcatData(Obj *dest, Obj &src, ConcatMode mode = ConcatMode::Error);

    // Convert a wide Unicode string to an UTF8 string

    inline std::string utf8_encode(const std::wstring wstr) {
        std::string utf8line;

        if (wstr.empty()) {
            return utf8line;
        }
        utf8line = std::wstring_convert < std::codecvt_utf8<wchar_t>>().to_bytes(wstr.c_str());
        return utf8line;
    }

    // Convert an UTF8 string to a wide Unicode String

    inline std::wstring utf8_decode(const std::string str) {
        std::wstring wide_line;

        if (str.empty()) {
            return wide_line;
        }
        wide_line = std::wstring_convert < std::codecvt_utf8<wchar_t>>().from_bytes(str.c_str());
        return wide_line;
    }

    ObjType getSummaryTensorType(ObjPtr & obj, ObjType start);
    std::vector<int64_t> getTensorSizes(Obj *obj);
    void calcTensorDims(ObjPtr & obj, std::vector<int64_t> &dims);
    void testTensorDims(ObjPtr & obj, at::IntArrayRef dims, int64_t pos);

    template<class... Ts> struct overloaded : Ts... {
        using Ts::operator()...;
        class Obj *obj;
    };
    // explicit deduction guide (not needed as of C++20)
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    class Obj : public Variable<ObjPtr>, public std::enable_shared_from_this<Obj> {
    public:

        //    constexpr static const char * BUILDIN_TYPE = "__var_type__";
        //    constexpr static const char * BUILDIN_NAME = "__var_name__";
        //    constexpr static const char * BUILDIN_BASE = "__class_base__";
        //    constexpr static const char * BUILDIN_CLASS = "__class_name__";
        //    constexpr static const char * BUILDIN_NAMESPACE = "__namespace__";

        Obj(ObjType type = ObjType::None, const char *var_name = nullptr, TermPtr func_proto = nullptr, ObjType fixed = ObjType::None) :
        m_var_type_current(type), m_var_name(var_name ? var_name : ""), m_func_proto(func_proto) {
            //        m_ctx = nullptr;
            m_is_const = false;
            m_check_args = false;
            m_ref_count = 0;
            m_func_ptr = nullptr;
            m_dimensions = nullptr;
            m_value = torch::empty({0}, isSimpleType(type) ? toTorchType(type) : at::ScalarType::Undefined);
            m_is_reference = false;
            m_func_abi = FFI_DEFAULT_ABI;
            m_var_type_fixed = fixed;
        }

        Obj(Context *ctx, const TermPtr term, bool as_value, Obj *local_vars);


        [[nodiscard]]
        static inline PairType ArgNull(const std::string name = "") {
            return Variable<ObjPtr>::pair(nullptr, name);
        }

        [[nodiscard]]
        static inline PairType Arg() {
            return pair(CreateNone());
        }

        [[nodiscard]]
        static inline PairType Arg(ObjPtr value, const std::string name = "") {
            return Variable<ObjPtr>::pair(value, name);
        }

        template<typename T>
        typename std::enable_if<std::is_pointer<T>::value || std::is_same<std::string, T>::value, PairType>::type
        static inline Arg(T value, const std::string name = "") {
            return pair(CreateString(value), name);
        }

        template<typename T>
        typename std::enable_if<!std::is_same<PairType, T>::value && !std::is_pointer<T>::value && !std::is_same<std::string, T>::value, PairType>::type
        static inline Arg(T value, const std::string name = "") {
            return Variable<ObjPtr>::pair(CreateValue(value, ObjType::None), name);
        }

        inline ObjPtr shared() {
            try {
                return shared_from_this();
            } catch (std::bad_weak_ptr &err) {
                LOG_RUNTIME("Exception thrown bad_weak_ptr! %s", err.what());
            }
        }

        ObjPtr MakeConst() {
            m_is_const = true;
            return shared();
        }

        ObjPtr MakeMutable() {
            m_is_const = false;
            return shared();
        }

#define TEST_CONST_() if (m_is_const) {LOG_RUNTIME("Can`t edit const value '%s'!", toString().c_str());}
#define TEST_INIT_() if (!m_var_is_init) {LOG_RUNTIME("Object not initialized '%s'!", toString().c_str());}

        [[nodiscard]]
        inline ObjType getType() {
            return m_var_type_current;
        }

        [[nodiscard]]
        inline ObjType getTypeAsLimit() {
            if (is_arithmetic_type()) {
                if (isIntegralType(m_var_type_current, true) && is_scalar()) {
                    return typeFromLimit(GetValueAsInteger());
                } else if (isFloatingType(m_var_type_current) && is_scalar()) {
                    return typeFromLimit(GetValueAsNumber());
                }
            } else if (is_type_name()) {
                return m_var_type_fixed;
            }
            return m_var_type_current;
        }

        [[nodiscard]]
        inline std::string & getName() {
            return m_var_name;
        }

        [[nodiscard]]
        inline const std::string & getName() const {
            return m_var_name;
        }

        /*
     Встроенные атрибуты класса

    Объекты класса — дочерние элементы по отношению к атрибутам самого языка Python. Таким образом они заимствуют некоторые атрибуты:
    Атрибут 	Описание
    __dict__ 	Предоставляет данные о классе коротко и доступно, в виде словаря
    __doc__ 	Возвращает строку с описанием класса, или None, если значение не определено
    __class__ 	Возвращает объект, содержащий информацию о классе с массой полезных атрибутов, включая атрибут __name__
    __module__ 	Возвращает имя «модуля» класса или __main__, если класс определен в выполняемом модуле.

    class Customer:
        'Это класс Customer'
        def __init__(self, name, phone, address):        
            self.name = name
            self.phone = phone
            self.address = address
 
  
    john = Customer("John",1234567, "USA")
 
    print ("john.__dict__ = ", john.__dict__)
    print ("john.__doc__ = ", john.__doc__)
    print ("john.__class__ = ", john.__class__)
    print ("john.__class__.__name__ = ", john.__class__.__name__) 
    print ("john.__module__ = ", john.__module__)  

    Вывод:

    john.__dict__ =  {'name': 'John', 'phone': 1234567, 'address': 'USA'}
    john.__doc__ =  Это класс Customer
    john.__class__ =  <class '__main__.Customer'>
    john.__class__.__name__ =  Customer
    john.__module__ =  __main__

         */
        void SetClassName(std::string &name) {
            m_class_name = name;
        }

        inline bool isConst() const {
            return m_is_const;
        }

        [[nodiscard]]
        inline bool is_none_type() const {
            return m_var_type_current == ObjType::None;
        }

        [[nodiscard]]
        inline bool is_bool_type() const {
            return isBooleanType(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_arithmetic_type() const {
            return isArithmeticType(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_string_type() const {
            return isString(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_dictionary_type() const {
            return isDictionary(m_var_type_current);
        }

        //Plain data — это неизменяемые структуры без ссылок на другие объекты.
        [[nodiscard]]
        inline bool is_plain_type() const {
            return isPlainDataType(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_other_type() const {
            return !(is_none_type() || is_bool_type() || is_arithmetic_type() || is_string_type() || is_dictionary_type());
        }

        [[nodiscard]]
        inline bool is_class() const {
            return isClass(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_simple() const {
            return isSimpleType(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_scalar() const {
            return is_tensor() && m_value.dim() == 0;
        }

        [[nodiscard]]
        inline bool is_function() const {
            return isFunction(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_tensor() const {
            return isTensor(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_integer() const {
            return isIntegralType(m_var_type_current, false);
        }

        [[nodiscard]]
        inline bool is_complex() const {
            return isComplexType(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_floating() const {
            return isFloatingType(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_indexing() const {
            return is_tensor() || is_string_type() || is_dictionary_type() || is_class();
        }

        [[nodiscard]]
        inline bool is_ellipsis() const {
            return isEllipsis(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_range() const {
            return isRange(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_type_name() const {
            return isTypeName(m_var_type_current);
        }

        [[nodiscard]]
        inline bool is_error() const {
            return m_var_type_current == ObjType::Error || m_var_type_fixed == ObjType::Error;
        }

        [[nodiscard]]
        inline bool is_return() const {
            return m_var_type_current == ObjType::Return || m_var_type_fixed == ObjType::Return;
        }

        [[nodiscard]]
        ObjType SummaryArithmeticType(ObjType type);

        [[nodiscard]]
        inline bool is_defined_type() {
            return m_var_type_fixed != ObjType::None;
        }

        inline void SetTermProp(Term &term);

        int64_t size() const override {
            return size(0);
        }
        int64_t size(int64_t ind) const;
        int64_t resize_(int64_t size, ObjPtr fill, const std::string = "");

        bool empty() const override {
            if (is_none_type()) {
                return true;
            } else if (m_var_type_current == ObjType::StrChar) {
                return !m_var_is_init || m_str.empty();
            } else if (m_var_type_current == ObjType::StrWide) {
                return !m_var_is_init || m_wstr.empty();
            } else if (is_tensor()) {
                return !m_var_is_init || at::_is_zerotensor(m_value);
            }
            return Variable<ObjPtr>::empty();
        }

        Variable<ObjPtr>::PairType & at(const std::string_view name) const {
            Obj * const obj = (Obj * const) this;
            return obj->Variable<ObjPtr>::at(name);
        }

        Variable<ObjPtr>::PairType & at(const std::string_view name) override {
            return Variable<ObjPtr>::at(name);
        }

        Variable<ObjPtr>::PairType & at(const int64_t index) override;
        const Variable<ObjPtr>::PairType & at(const int64_t index) const override;

        Variable<ObjPtr>::PairType & at(ObjPtr find) {
            if (!find->is_string_type()) {
                LOG_RUNTIME("Value must be a string type %d", (int) find->getType());
            }
            return Variable<ObjPtr>::at(find->GetValueAsString());
        }

        bool exist(ObjPtr &find, bool strong);
        size_t ItemValueCount(ObjPtr &find, bool strong);

        static bool is_true(const char *text) {
            std::string temp(text);
            std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
            return temp.compare("true") == 0;
        }

        /**
         * Создать копию объекта (клонировать)
         * @return 
         */
        ObjPtr operator()(Context *ctx) {
            Obj args;
            return Call(ctx, &args);
        }

        template <typename... T>
        typename std::enable_if<is_all<Obj::PairType, T ...>::value, ObjPtr>::type
        inline operator()(Context *ctx, T ... args) {
            auto list = {args...};
            ObjPtr arg = Obj::CreateDict();
            for (auto &elem : list) {
                arg->push_back(elem);
            }
            return Call(ctx, arg.get());
        }

        inline ObjPtr Call(Context *ctx) {
            Obj args;
            return Call(ctx, &args);
        }

        template <typename... T>
        typename std::enable_if<is_all<Obj::PairType, T ...>::value, ObjPtr>::type
        inline Call(Context *ctx, T ... args) {
            auto list = {args...};
            Obj arg;
            for (auto &elem : list) {
                arg.push_back(elem);
            }
            return Call(ctx, &arg);
        }

        ObjPtr Call(Context *ctx, Obj *args);





        //унарный плюс ничего не делает.

        ObjPtr operator+() {
            if (is_arithmetic_type()) {
                return shared();
            }
            LOG_RUNTIME("Unary plus for object '%s' not supported!", toString().c_str());
        }

        ObjPtr operator-() {
            if (is_arithmetic_type()) {
                if (is_tensor()) {
                    m_value = -m_value;
                } else if (is_integer()) {
                    SetValue_(-GetValueAsInteger());
                } else if (isFloatingType(m_var_type_current)) {
                    SetValue_(-GetValueAsNumber());
                } else {
                    LOG_RUNTIME("Unary minus for object '%s' not supported!", toString().c_str());
                }
                return shared();
            }
            LOG_RUNTIME("Unary minus for object '%s' not supported!", toString().c_str());
        }

        ObjPtr & operator+(ObjPtr & obj) {

            if (is_tensor()) {
                return obj;
            }
            LOG_RUNTIME("Object '%s' not numeric!", obj->toString().c_str());
        }

        ObjPtr &operator-(ObjPtr & obj) {
            if (is_tensor()) {
                obj->m_value = torch::zeros_like(obj->m_value) - obj->m_value;
                return obj;
            }
            LOG_RUNTIME("Object '%s' not numeric!", obj->toString().c_str());
        }


        //префиксная версия возвращает значение после инкремента

        ObjPtr operator++() {
            if (is_tensor()) {
                m_value.add_(torch::ones_like(m_value));

                return shared();
            }
            LOG_RUNTIME("Object '%s' not numeric!", toString().c_str());
        }

        //постфиксная версия возвращает значение до инкремента

        ObjPtr operator++(int) {
            ObjPtr old = Clone();
            Obj::operator++();
            return old;
        }

        //префиксная версия возвращает значение после декремента

        ObjPtr operator--() {
            if (is_tensor()) {
                m_value.sub_(torch::ones_like(m_value));
                return shared();
            }
            LOG_RUNTIME("Object '%s' not numeric!", toString().c_str());
        }

        //постфиксная версия возвращает значение до декремента

        ObjPtr operator--(int) {
            ObjPtr old = Clone();
            Obj::operator--();
            return old;
        }

        inline ObjPtr operator*(ObjPtr obj) {
            ASSERT(obj);
            return operator*(*obj);
        }

        inline ObjPtr operator*(Obj value) {
            ObjPtr result = Clone();
            *result *= value;
            return result;
        }

        inline ObjPtr operator/(ObjPtr obj) {
            ASSERT(obj);
            return operator/(*obj);
        }

        inline ObjPtr operator/(Obj value) {
            ObjPtr result = Clone();
            *result /= value;
            return result;
        }

        inline ObjPtr op_div_ceil(ObjPtr obj) {
            ASSERT(obj);
            return op_div_ceil(*obj);
        }

        inline ObjPtr op_div_ceil(Obj value) {
            ObjPtr result = Clone();
            result->op_div_ceil_(value);
            return result;
        }

        inline ObjPtr op_concat(ObjPtr obj, ConcatMode mode = ConcatMode::Error) {
            ASSERT(obj);
            return op_concat(*obj, mode);
        }

        inline ObjPtr op_concat(Obj &value, ConcatMode mode = ConcatMode::Error) {
            ObjPtr result = Clone();
            result->op_concat_(value, mode);
            return result;
        }

        inline ObjPtr op_concat_(Obj &obj, ConcatMode mode = ConcatMode::Error) {
            ConcatData(this, obj, mode);
            return shared();
        }

        inline ObjPtr op_concat_(ObjPtr obj, ConcatMode mode = ConcatMode::Error) {
            return op_concat_(*obj, mode);
        }

        inline ObjPtr operator%(ObjPtr obj) {
            ASSERT(obj);
            return operator%(*obj);
        }

        inline ObjPtr operator%(Obj value) {
            ObjPtr result = Clone();
            *result %= value;
            return result;
        }

        inline ObjPtr operator+(ObjPtr obj) {
            ASSERT(obj);
            return operator+(*obj);
        }

        inline ObjPtr operator+(Obj value) {
            ObjPtr result = Clone();
            *result += value;
            return result;
        }

        inline ObjPtr operator-(ObjPtr obj) {
            ASSERT(obj);
            return operator-(*obj);
        }

        inline ObjPtr operator-(Obj value) {
            ObjPtr result = Clone();
            *result -= value;
            return result;
        }

        //    ObjPtr op_lshift(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '<<' not implementd!");
        //    }
        //
        //    ObjPtr op_rshift(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '>>' not implementd!");
        //    }
        //
        //    ObjPtr op_rrshift(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '>>>' not implementd!");
        //    }

        inline bool operator<(ObjPtr obj) {
            ASSERT(obj);
            return operator<(*obj);
        }

        inline bool operator<(Obj obj) {
            return op_compare(obj) < 0;
        }

        inline bool operator<=(ObjPtr obj) {
            ASSERT(obj);
            return operator<=(*obj);
        }

        inline bool operator<=(Obj obj) {
            return op_compare(obj) <= 0;
        }

        inline bool operator>(ObjPtr obj) {
            ASSERT(obj);
            return operator>(*obj);
        }

        inline bool operator>(Obj obj) {
            return op_compare(obj) > 0;
        }

        inline bool operator>=(ObjPtr obj) {
            ASSERT(obj);
            return operator>=(*obj);
        }

        inline bool operator>=(Obj obj) {
            return op_compare(obj) >= 0;
        }
        int op_compare(Obj & value);

        /*
         *  instanceof (проверка класса объекта) test_obj ~~ obj  объекты совместимы по свойствим и их типам (утиная типизация)
         *  in (проверка существования свойства)  test_obj ~~ (prop=,) объект содержит указанные свойства (т.к. пустой тип совместим с любым типом)
         *  
         * Реализация модели наследования:
         * ~ - проверка по классу объекта, т.е. проверка имени объекта или имени базового типа (объект должен быть наследником образца)
         * Реализация утиной типизации:
         * ~~ - сравнение с образцом на совместимость типов (пустой тип совместим с любым)
         * ~~~ - сравнение с образцом на равенство типов, т.е. типы объекта должны быть идентичны образцу, включая пустой
         * Для двух последних случаев сравнение производится только для одного уровня (без раскрытия сложенных словарей, если они присутствуют в образце)
         * или сравнение должно зависить от того, есть ли в образце вложенные словари (классы) ??????????????
         */

        bool op_class_test(ObjPtr obj, Context *ctx) const;
        bool op_class_test(const char * name, Context *ctx) const;

        inline bool op_duck_test(ObjPtr obj, bool strong) {
            ASSERT(obj);
            return op_duck_test(obj.get(), strong);
        }
        bool op_duck_test(Obj *value, bool strong);
        static bool op_duck_test_prop(Obj *base, Obj *value, bool strong);

        inline bool op_equal(ObjPtr value) {
            ASSERT(value);
            if (value) {
                return op_equal(*value);
            }
            return false;
        }

        inline ObjPtr operator==(ObjPtr obj) {
            ASSERT(obj);
            return operator==(*obj);
        }

        inline ObjPtr operator==(Obj obj) {
            return op_equal(obj) ? Obj::Yes() : Obj::No();
        }
        bool op_equal(Obj & value);

        inline bool op_accurate(ObjPtr obj) {
            ASSERT(obj);
            return op_accurate(*obj);
        }
        bool op_accurate(Obj & value);

        inline ObjPtr operator!=(ObjPtr obj) {
            ASSERT(obj);
            return operator!=(*obj);
        }

        inline ObjPtr operator!=(Obj obj) {
            return op_equal(obj) ? Obj::No() : Obj::Yes();
        }

        inline ObjPtr op_bit_and(ObjPtr obj, bool strong) {
            ASSERT(obj);
            return op_bit_and(*obj, strong);
        }

        inline ObjPtr op_bit_and(Obj &obj, bool strong) {
            ObjPtr result = Clone();
            result->op_bit_and_set(obj, strong);
            return result;
        }

        inline ObjPtr op_pow(ObjPtr obj) const {
            ASSERT(obj);
            return op_pow(*obj);
        }

        inline ObjPtr op_pow(Obj &obj) const {
            ObjPtr result = Clone();
            result->op_pow_(obj);
            return result;
        }

        inline ObjPtr op_pow_(ObjPtr obj) {
            ASSERT(obj);
            return op_pow_(*obj);
        }

        ObjPtr op_pow_(Obj &obj);

        //    ObjPtr operator^(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '^' not implementd!");
        //    }
        //
        //    ObjPtr operator|(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '|' not implementd!");
        //    }
        //
        //    ObjPtr operator&&(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '&&' not implementd!");
        //    }
        //
        //    ObjPtr operator||(ObjPtr obj) {
        //        LOG_RUNTIME("Operator '||' not implementd!");
        //    }

        ObjPtr op_assign(ObjPtr obj) {
            if (obj) {
                return op_assign(*obj);
            }
            clear_();
            return shared();
        }

        ObjPtr op_assign(Obj & obj) {
            obj.CloneDataTo(*this);
            obj.ClonePropTo(*this);
            return shared();
        }

        inline bool operator=(Obj & obj) {
            obj.CloneDataTo(*this);
            return true;
        }

        /*    
         * ?: (выбор из двух операндов)
         * = (присваивание)
         * *=, /=, %=, +=, -=, &=, ^=, |=, <<=, >>=, >>>= (операции с присваиванием)
         * , (отбрасывание первого и возврат второго операнда) 
         */


        inline ObjPtr operator*=(ObjPtr obj) {
            ASSERT(obj);
            return operator*=(*obj);
        }

        ObjPtr operator*=(Obj obj);

        inline ObjPtr operator/=(ObjPtr obj) {
            ASSERT(obj);
            return operator/=(*obj);
        }

        ObjPtr operator/=(Obj obj);

        inline ObjPtr op_div_ceil_(ObjPtr obj) {
            ASSERT(obj);
            return op_div_ceil_(*obj);
        }

        ObjPtr op_div_ceil_(Obj &obj);

        inline ObjPtr operator%=(ObjPtr obj) {
            ASSERT(obj);
            return operator%=(*obj);
        }

        ObjPtr operator%=(Obj obj);

        inline ObjPtr operator+=(ObjPtr obj) {
            ASSERT(obj);
            return operator+=(*obj);
        }
        ObjPtr operator+=(Obj obj);

        inline ObjPtr operator-=(ObjPtr obj) {
            ASSERT(obj);
            return operator-=(*obj);
        }
        ObjPtr operator-=(Obj obj);


        ObjPtr op_bit_and_set(Obj &obj, bool strong);

        inline ObjPtr operator^=(ObjPtr obj) {
            ASSERT(obj);
            return operator^=(*obj);
        }

        ObjPtr operator^=(Obj obj) {
            LOG_RUNTIME("Operator '^=' not implementd!");
        }

        inline ObjPtr operator|=(ObjPtr obj) {
            ASSERT(obj);
            return operator|=(*obj);
        }

        ObjPtr operator|=(Obj obj) {
            LOG_RUNTIME("Operator '|=' not implementd!");
        }

        inline ObjPtr op_lshift_set(ObjPtr obj) {
            ASSERT(obj);
            return op_lshift_set(*obj);
        }

        ObjPtr op_lshift_set(Obj obj) {
            LOG_RUNTIME("Operator '<<=' not implementd!");
        }

        inline ObjPtr op_rshift_set(ObjPtr obj) {
            ASSERT(obj);
            return op_rshift_set(*obj);
        }

        ObjPtr op_rshift_set(Obj obj) {
            LOG_RUNTIME("Operator '>>=' not implementd!");
        }

        inline ObjPtr op_rrshift_set(ObjPtr obj) {
            ASSERT(obj);
            return op_rrshift_set(*obj);
        }

        ObjPtr op_rrshift_set(Obj obj) {
            LOG_RUNTIME("Operator '>>>=' not implementd!");
        }

        template <typename T>
        bool operator=(T value) {
            SetValue_(value);
            return true;
        }


        static ObjPtr GetIndex(ObjPtr obj, TermPtr index_arg);

        static ObjPtr GetIndex(ObjPtr obj, size_t index) {
            return (*obj)[index];
        }

        void dump_dict_(std::string &str, bool deep = true) const {
            bool first = true;
            for (auto &elem : * this) {
                if (first) {
                    first = false;
                } else {
                    str.append(", ");
                }
                if (elem.first.empty()) {
                    if (elem.second) {
                        str.append(elem.second->toString(false));
                    } else {
                        str.append("_");
                    }
                } else {
                    if (elem.second) {

                        if (deep || !(elem.second->is_tensor() || elem.second->getType() == ObjType::Class)) {
                            str.append(elem.first);
                            str.append("=");
                            str.append(elem.second->toString(false));
                        } else if (elem.second->getType() == ObjType::Class && elem.second->m_is_reference) {
                            str.append("&");
                            str.append(elem.second->getName());
                        } else {

                            str.append(elem.second->getName());
                            str.append("=");
                            str.append(elem.second->toString());
                        }
                    } else {
                        str.append("_");
                    }
                }
            }
        }

        std::string toString(bool deep = true) const;

        std::string GetValueAsString() const;

        inline std::wstring GetValueAsStringWide() const {
            return utf8_decode(GetValueAsString());
        }

        inline int64_t GetValueAsInteger() const {
            TEST_INIT_();
            switch (m_var_type_current) {
                case ObjType::Bool:
                case ObjType::Char:
                case ObjType::Short:
                case ObjType::Int:
                case ObjType::Long:
                case ObjType::Integer:
                    return m_value.toType(at::ScalarType::Long).item<int64_t>();

                case ObjType::Float:
                case ObjType::Double:
                case ObjType::Number:
                    return static_cast<int64_t> (m_value.item<double>());

                case ObjType::Fraction:
                    ASSERT(m_fraction);
                    return m_fraction->GetAsInteger();

                case ObjType::StrWide:
                    return std::stoll(m_wstr);
                case ObjType::StrChar:
                    return std::stoll(m_str);
                default:
                    if (m_var_type_current == ObjType::Pointer || m_var_type_fixed == ObjType::Pointer) {
                        return reinterpret_cast<int64_t> (m_func_ptr);
                    }
                    LOG_RUNTIME("Data type incompatible %s", toString().c_str());
            }
            return 0;
        }

        inline double GetValueAsNumber() const {
            TEST_INIT_();
            switch (m_var_type_current) {
                case ObjType::Float:
                case ObjType::Double:
                    return m_value.item<double>();

                case ObjType::Fraction:
                    ASSERT(m_fraction);
                    return m_fraction->GetAsNumber();

                case ObjType::StrWide:
                    return std::stod(m_wstr);
                case ObjType::StrChar:
                    return std::stod(m_str);

                default:
                    if (is_simple()) {
                        return static_cast<double> (GetValueAsInteger());
                    }
            }
            LOG_RUNTIME("Data type incompatible %s", toString().c_str());
        }

        inline bool GetValueAsBoolean() const {
            if (!m_var_is_init) {
                return false;
            }
            if (is_scalar()) {
                return m_value.toType(at::ScalarType::Bool).item<double>();
            } else if (isSimpleType(m_var_type_current)) {
                // Error: Boolean value of Tensor with more than one value is ambiguous
                return !at::_is_zerotensor(m_value);
            } else {
                switch (m_var_type_current) {
                    case ObjType::StrWide:
                        return !m_wstr.empty();
                    case ObjType::StrChar:
                        return !m_str.empty();
                    case ObjType::None:
                        return false;

                    case ObjType::Fraction:
                        ASSERT(m_fraction);
                        return m_fraction->GetAsInteger();

                    case ObjType::Dictionary:
                    case ObjType::Class:
                        if (size()) {
                            return true;
                        }
                        for (auto &elem : m_class_parents) {
                            if (elem->GetValueAsBoolean()) {
                                return true;
                            }
                        }
                        return false;

                    default:
                        LOG_RUNTIME("Type cast to bool %s", toString().c_str());
                }
                return true;
            }
        }

        at::Scalar toTorchScalar() {
            at::Scalar result;
            if (is_integer() || is_bool_type()) {
                result = at::Scalar(GetValueAsInteger());
            } else if (is_floating()) {
                result = at::Scalar(GetValueAsNumber());
            } else {
                LOG_RUNTIME("Not support Torch ScalarType '%s'", newlang::toString(m_var_type_current));
            }
            return result;

        }

        static ObjPtr CreateType(ObjType type, const char *var_name = nullptr, ObjType fixed = ObjType::None) {
            TermPtr term = nullptr;
            return std::make_shared<Obj>(type, var_name, term, fixed);
        }

        static ObjPtr CreateFraction(const std::string_view val) {

            std::string str;
            std::remove_copy(val.begin(), val.end(), back_inserter(str), '_');

            if (str.empty()) {
                LOG_RUNTIME("Empty string!");
            }

            ObjPtr frac = Obj::CreateType(ObjType::Fraction);

            // Ищем разделитель дроби
            size_t pos = str.find("\\");

            // Если символ не найден - то вся строка является числом 
            if (pos == std::string::npos) {
                frac->m_fraction = std::make_shared<Fraction>(str, "1");
            } else {
                // Числитель - левая часть
                // Знаменатель - правая часть
                frac->m_fraction = std::make_shared<Fraction>(str.substr(0, pos), str.substr(pos + 1, str.length()));
                // Знаменатель не должен быть равен нулю
                if (frac->m_fraction->m_denominator.isZero()) {
                    LOG_RUNTIME("Denominator must be different from zero!");
                }
            }

            frac->m_var_is_init = true;
            if (str.compare(frac->m_fraction->GetAsString().c_str()) != 0) {
                LOG_RUNTIME("Fraction value '%s' does not match source string  '%s'!", frac->m_fraction->GetAsString().c_str(), str.c_str());
            }

            return frac;
        }


        // чистая функция
        static ObjPtr BaseTypeConstructor(const Context *ctx, Obj &in);
        static ObjPtr ConstructorSimpleType_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorDictionary_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorNative_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorClass_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorStruct_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorEnum_(const Context *ctx, Obj & args);

        static ObjPtr ConstructorError_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorReturn_(const Context *ctx, Obj & args);
        static ObjPtr ConstructorInterraption_(const Context *ctx, Obj & args, ObjType type);

        static ObjPtr CreateBaseType(ObjType type);

        static ObjPtr CreateNone() {
            return CreateType(ObjType::None);
        }

        template <typename T1, typename T2, typename T3>
        static ObjPtr CreateRange(T1 start, T2 stop, T3 step) {
            ObjPtr obj = CreateType(ObjType::Range);
            obj->push_back(CreateValue(start, ObjType::None), "start");
            obj->push_back(CreateValue(stop, ObjType::None), "stop");
            obj->push_back(CreateValue(step, ObjType::None), "step");
            obj->m_var_is_init = true;
            return obj;
        }

        template <typename T1, typename T2>
        static ObjPtr CreateRange(T1 start, T2 stop) {
            ObjPtr obj = CreateType(ObjType::Range);
            obj->push_back(CreateValue(start, ObjType::None), "start");
            obj->push_back(CreateValue(stop, ObjType::None), "stop");
            if (start < stop) {
                obj->push_back(CreateValue(1, ObjType::None), "step");
            } else {
                obj->push_back(CreateValue(-1, ObjType::None), "step");
            }
            obj->m_var_is_init = true;
            return obj;
        }

        static ObjType GetType(torch::Tensor & val) {
            switch (val.dtype().toScalarType()) {
                case at::ScalarType::Bool:
                    return ObjType::Bool;
                case at::ScalarType::Half:
                case at::ScalarType::BFloat16:
                case at::ScalarType::Float:
                    return ObjType::Float;
                case at::ScalarType::Double:
                    return ObjType::Double;
                case at::ScalarType::Byte:
                case at::ScalarType::Char:
                case at::ScalarType::QInt8:
                case at::ScalarType::QUInt8:
                case at::ScalarType::QUInt4x2:
                    return ObjType::Char;
                case at::ScalarType::Short:
                    return ObjType::Short;
                case at::ScalarType::Int:
                case at::ScalarType::QInt32:
                    return ObjType::Int;
                case at::ScalarType::Long:
                    return ObjType::Long;
                case at::ScalarType::ComplexHalf:
                case at::ScalarType::ComplexFloat:
                    return ObjType::ComplexFloat;
                case at::ScalarType::ComplexDouble:
                    return ObjType::ComplexDouble;
            }
            LOG_RUNTIME("Fail tensor type %s", val.toString().c_str());
        }

        static ObjPtr CreateBool(bool value, const char *var_name = nullptr) {
            ObjPtr result = CreateType(ObjType::None, var_name);
            result->m_value = torch::scalar_tensor(value, torch::Dtype::Bool);
            result->m_var_type_current = ObjType::Bool;
            result->m_var_is_init = true;
            return result;
        }

        static ObjPtr CreateTensor(torch::Tensor tensor, const char *var_name = nullptr) {
            ObjType check_type = GetType(tensor);
            if (!isTensor(check_type)) {
                LOG_RUNTIME("Unsupport torch type %s (%d)!", at::toString(tensor.dtype().toScalarType()), (int) tensor.dtype().toScalarType());
            }
            ObjPtr result = CreateType(check_type, var_name);
            result->m_value = tensor;
            result->m_var_is_init = true;
            return result;
        }

        static ObjPtr CreateTensor(ObjPtr data, ObjType type) {
            torch::Tensor var = ConvertToTensor(data.get(), toTorchType(type), false);
            //        ConvertToTensor(data->index_get({0}).get(), type, false);
            return CreateTensor(var);
        }

        inline torch::Tensor toTensor() {
            return ConvertToTensor(this);
        }

        inline torch::Tensor & asTensor_() {
            NL_CHECK(is_tensor(), "Fail type as Tensor");
            return m_value;
        }

        at::indexing::Slice toSlice() {
            NL_CHECK(is_range(), "Convert to slice supported for range only!");

            ASSERT(size() == 3 && Variable<ObjPtr>::at("start").second && Variable<ObjPtr>::at("stop").second && Variable<ObjPtr>::at("step").second);

            NL_CHECK(Variable<ObjPtr>::at("start").second->is_integer(), "Slice value start support integer type only!");
            NL_CHECK(Variable<ObjPtr>::at("stop").second->is_integer(), "Slice value stop support integer type only!");
            NL_CHECK(Variable<ObjPtr>::at("step").second->is_integer(), "Slice value step support integer type only!");

            return at::indexing::Slice(
                    Variable<ObjPtr>::at("start").second->GetValueAsInteger(),
                    Variable<ObjPtr>::at("stop").second->GetValueAsInteger(),
                    Variable<ObjPtr>::at("step").second->GetValueAsInteger());
        }

        /*
         * From TensorIndexing.h
    // There is one-to-one correspondence between Python and C++ tensor index types:
    // Python                  | C++
    // -----------------------------------------------------
    // `None`                  | `at::indexing::None`
    // `Ellipsis`              | `at::indexing::Ellipsis`
    // `...`                   | `"..."`
    // `123`                   | `123`
    // `True` / `False`        | `true` / `false`
    // `:`                     | `Slice()` / `Slice(None, None)`
    // `::`                    | `Slice()` / `Slice(None, None, None)`
    // `1:`                    | `Slice(1, None)`
    // `1::`                   | `Slice(1, None, None)`
    // `:3`                    | `Slice(None, 3)`
    // `:3:`                   | `Slice(None, 3, None)`
    // `::2`                   | `Slice(None, None, 2)`
    // `1:3`                   | `Slice(1, 3)`
    // `1::2`                  | `Slice(1, None, 2)`
    // `:3:2`                  | `Slice(None, 3, 2)`
    // `1:3:2`                 | `Slice(1, 3, 2)`
    // `torch.tensor([1, 2])`) | `torch::tensor({1, 2})`
         */
        Index toIndex() {
            if (is_none_type()) {
                return Index(c10::nullopt);
            } else if (is_dictionary_type()) {
                std::vector<int64_t> temp = toIntVector(true);
                if (temp.size()) {
                    torch::Tensor tensor = torch::from_blob(temp.data(), temp.size(), torch::Dtype::Long);
                    return Index(tensor.clone());
                } else {
                    return Index(c10::nullopt);
                }

            } else if (is_scalar()) {
                switch (m_var_type_current) {
                    case ObjType::Bool:
                        return Index(GetValueAsBoolean());

                    case ObjType::Char:
                    case ObjType::Short:
                    case ObjType::Int:
                    case ObjType::Long:
                        return Index(GetValueAsInteger());
                    default:
                        LOG_RUNTIME("Fail convert scalar type '%s' to Index!", newlang::toString(m_var_type_current));
                }
            } else if (is_tensor()) {
                return Index(m_value);
            } else if (is_ellipsis()) {
                return Index(at::indexing::Ellipsis);
            } else if (is_range()) {
                return Index(toSlice());
            }
            LOG_RUNTIME("Fail convert object '%s' to Index!", toString().c_str());
        }

        inline operator bool () const {
            return GetValueAsBoolean();
        }

        inline operator at::Tensor() {
            return ConvertToTensor(this);
        }

        inline operator at::TensorOptions() const {
            return ConvertToTensorOptions(this);
        }

        inline operator at::DimnameList() const {
            return ConvertToDimnameList(this);
        }

        std::vector<int64_t> toIntVector(bool raise = true) const {
            std::vector<int64_t> result;
            for (int i = 0; i < size(); i++) {
                if (raise && !at(i).second->is_integer()) {
                    LOG_RUNTIME("Item does not contain an integer value! '%s'", at(i).second->GetValueAsString().c_str());
                }
                result.push_back(at(i).second->GetValueAsInteger());
            }
            return result;
        }


        //  ПЕРЕКРЫВАЕТ ДРУГИЕ МЕТОДЫ  !!!!!!!!!!!!!! at::TensorOptions()
        //    inline operator std::string () const {
        //        return GetValueAsString();
        //    }

        template <typename T>
        typename std::enable_if<std::is_integral<T>::value, ObjPtr>::type
        static CreateValue(T value, ObjType fix_type = ObjType::None) {
            ObjPtr result = CreateType(fix_type);
            result->m_var_type_fixed = fix_type;
            result->m_var_type_current = typeFromLimit((int64_t) value);
            if (fix_type != ObjType::None) {
                NL_CHECK(canCast(result->m_var_type_current, fix_type),
                        "Fail cast type from '%s' to '%s'!", newlang::toString(result->m_var_type_current), newlang::toString(fix_type));
            }
            result->m_value = torch::scalar_tensor(value, toTorchType(result->m_var_type_current));
            result->m_var_is_init = true;
            return result;
        }

        template <typename T>
        typename std::enable_if<std::is_floating_point<T>::value, ObjPtr>::type
        static CreateValue(T value, ObjType fix_type = ObjType::None) {
            ObjPtr result = CreateType(fix_type);
            result->m_var_type_fixed = fix_type;
            result->m_var_type_current = typeFromLimit((double) value);
            if (fix_type != ObjType::None) {
                NL_CHECK(canCast(result->m_var_type_current, fix_type),
                        "Fail cast type from '%s' to '%s'!", newlang::toString(result->m_var_type_current), newlang::toString(fix_type));
            }
            result->m_value = torch::scalar_tensor(value, toTorchType(result->m_var_type_current));
            result->m_var_is_init = true;
            return result;
        }

        static ObjPtr CreateString(const std::string str) {
            ObjPtr result = CreateType(ObjType::StrChar);
            result->m_str = str;
            result->m_var_type_fixed = ObjType::String;
            result->m_var_is_init = true;
            return result;
        }

        static ObjPtr CreateString(const std::wstring str) {
            ObjPtr result = CreateType(ObjType::StrWide);
            result->m_wstr = str;
            result->m_var_type_fixed = ObjType::String;
            result->m_var_is_init = true;
            return result;
        }

        inline static ObjPtr Yes() {
            ObjPtr result = std::make_shared<Obj>(ObjType::Bool);
            bool value = true;
            result->m_value = torch::scalar_tensor(value);
            result->m_var_is_init = true;
            return result->MakeConst();
        }

        inline static ObjPtr No() {
            ObjPtr result = std::make_shared<Obj>(ObjType::Bool);
            bool value = false;
            result->m_value = torch::scalar_tensor(value);
            result->m_var_is_init = true;
            return result->MakeConst();
        }

        inline static ObjPtr CreateDict() {
            return Obj::CreateType(ObjType::Dictionary);
        }

        template <typename... T>
        typename std::enable_if<is_all<PairType, T ...>::value, ObjPtr>::type
        static CreateDict(T ... args) {
            std::array < PairType, sizeof...(args) > list = {args...};
            ObjPtr result = Obj::CreateType(ObjType::Dictionary);
            for (auto &elem : list) {
                result->push_back(elem);
            }
            result->m_var_is_init = true;
            return result;
        }

        inline static ObjPtr CreateClass(std::string name) {
            ObjPtr result = Obj::CreateType(ObjType::Class);
            result->m_class_name = name;
            result->m_var_is_init = true;
            return result;
        }

        template <typename... T>
        typename std::enable_if<is_all<PairType, T ...>::value, ObjPtr>::type
        static CreateClass(std::string name, T ... args) {
            std::array < PairType, sizeof...(args) > list = {args...};
            ObjPtr result = Obj::CreateType(ObjType::Class);
            result->m_class_name = name;
            for (auto &elem : list) {
                result->push_back(elem);
            }
            result->m_var_is_init = true;
            return result;
        }


        ObjPtr CallNative(Context *ctx, Obj args);

        ObjPtr Clone(const char *new_name = nullptr) const {
            ObjPtr clone = Obj::CreateNone();
            CloneDataTo(*clone);
            ClonePropTo(*clone);
            if (new_name) {
                clone->m_var_name = new_name;
            }
            clone->m_is_const = false;
            clone->m_ref_count = 0;
            return clone;
        }

        inline void CloneTo(Obj & clone) {
            if (&clone == this) {
                // Не клонировать сам в себя
                return;
            }
            CloneDataTo(clone);
            ClonePropTo(clone);
        }

        inline void CloneTo(ObjPtr & clone) {
            if (clone.get() == this) {
                // Не клонировать сам в себя
                return;
            }
            clone.reset();
            clone = Obj::CreateNone();
            CloneDataTo(*clone);
            ClonePropTo(*clone);
        }

        void CloneDataTo(Obj & clone) const;
        void ClonePropTo(Obj & clone) const;

        virtual ~Obj() {
            //        Clear();
        }

        ObjPtr toType(ObjType target) const {
            ObjPtr result = Clone();
            result->toType_(target);
            return result;
        }

        ObjPtr toType_(Obj *type);
        void toType_(ObjType type);

        ObjPtr toShape(ObjPtr dims) const {
            ObjPtr result = Clone();
            result->toShape_(dims);
            return result;
        }
        ObjPtr toShape_(ObjPtr shape);

        ObjPtr Convert(ObjType type, ObjPtr shape = nullptr) const {
            ObjPtr result = Clone();
            result->Convert_(type, shape);
            return result;
        }

        ObjPtr Convert_(ObjType type, ObjPtr shape = nullptr) {
            if (shape) {
                toShape_(shape);
            }
            toType_(type);
            return shared();
        }

        const ObjPtr index_get(const std::vector<Index> & index) const;

        inline ObjPtr index_set(const std::vector<Index> & index, const ObjPtr value) const {
            ObjPtr result = Clone();
            result->index_set_(index, value);
            return result;
        }
        ObjPtr index_set_(const std::vector<Index> & index, const ObjPtr value);

        inline ObjPtr op_set_index(ObjPtr index, ObjPtr value) {
            return op_set_index(index->GetValueAsInteger(), value->GetValueAsString());
        }
        ObjPtr op_set_index(int64_t index, std::string value);

        template < typename T>
        typename std::enable_if<std::is_same<T, bool>::value, void>::type
        SetValue_(bool value) {

            TEST_CONST_();

            ASSERT(m_var_type_current != ObjType::Class);
            clear_();
            m_value = torch::scalar_tensor(value, torch::Dtype::Bool);
            m_var_type_current = GetType(m_value);
            m_var_is_init = true;
        }

        template < typename T>
        typename std::enable_if<std::is_integral<T>::value, void>::type
        SetValue_(T value) {
            TEST_CONST_();
            ASSERT(m_var_type_current != ObjType::Class);
            clear_();
            m_value = torch::scalar_tensor(value, toTorchType(typeFromLimit((int64_t) value)));
            m_var_type_current = GetType(m_value);
            m_var_is_init = true;
        }

        template < typename T>
        typename std::enable_if<std::is_floating_point<T>::value, void>::type
        SetValue_(T value) {

            TEST_CONST_();
            ASSERT(m_var_type_current != ObjType::Class);
            clear_();
            m_value = torch::scalar_tensor(value, toTorchType(typeFromLimit(value)));
            m_var_type_current = GetType(m_value);
            m_var_is_init = true;
        }

        template < typename T>
        typename std::enable_if<std::is_same<char *, T>::value, void>::type
        SetValue_(T text) {
            std::string str(text);
            SetValue_(text);
        }

        template < typename T>
        typename std::enable_if<std::is_same<wchar_t *, T>::value, void>::type
        SetValue_(T text) {
            std::wstring str(text);
            SetValue_(text);
        }

        void SetValue_(std::string text) {
            TEST_CONST_();
            if (m_var_type_current != ObjType::StrChar) {
                testConvertType(ObjType::StrChar);
                m_var_type_current = ObjType::StrChar;
            }
            m_str.swap(text);
            m_var_is_init = true;
        }

        void SetValue_(std::wstring text) {
            TEST_CONST_();
            if (m_var_type_current != ObjType::StrWide) {
                testConvertType(ObjType::StrWide);
                m_var_type_current = ObjType::StrWide;
            }
            m_wstr.swap(text);
            m_var_is_init = true;
        }

        inline void testConvertType(ObjType type) {
            if (m_var_type_fixed == ObjType::None || canCast(type, m_var_type_fixed)) {
                return;
            }
            LOG_RUNTIME("Cannot changed type from '%s' to '%s'!", newlang::toString(type), newlang::toString(m_var_type_fixed));
        }

        inline void testResultIntegralType(ObjType type, bool upscalint) {
            ObjType new_type = m_var_type_current;
            if (!canCast(type, m_var_type_current)) {
                testConvertType(type);
                new_type = type;
            }
            bool check = false;
            if (upscalint && isIntegralType(m_var_type_current, true) && isIntegralType(type, true)) {
                if (type < ObjType::Long) {
                    new_type = static_cast<ObjType> (static_cast<uint8_t> (type) + 1);
                    check = true;
                }
                if (check && m_var_type_fixed != ObjType::None && !canCast(new_type, m_var_type_fixed)) {
                    new_type = type; // Тип данных менять нельзя, но сама операция возможна
                    LOG_WARNING("Data type '%s' cannot be changed to '%s', loss of precision is possible!", newlang::toString(type), newlang::toString(m_var_type_fixed));
                }
            }
            if (new_type != m_var_type_current) {
                m_value = m_value.toType(toTorchType(new_type));
                m_var_type_current = new_type;
            }
        }

        void SetValue_(ObjPtr value) {
            TEST_CONST_();
            if (value->is_none_type()) {
                clear_();
                return;
            } else if ((is_none_type() || is_tensor()) && value->is_tensor()) {

                if (value->empty()) {
                    m_value.reset();
                    m_var_is_init = false;
                    return;
                }

                if (!canCast(value->m_var_type_current, m_var_type_current)) {
                    testConvertType(value->m_var_type_current);
                    toType_(value->getType());
                }
                if (is_none_type()) {
                    m_value = value->m_value;
                    m_var_type_current = value->m_var_type_current;
                } else {

                    if (m_value.dim() == 0 && value->m_value.dim() != 0) {
                        LOG_RUNTIME("Fail assign tensor to scalar!");
                    }
                    if (!m_var_is_init) {
                        m_value = value->m_value.clone();
                    } else {
                        if (!m_value.sizes().equals(value->m_value.sizes()) && value->m_value.dim() != 0) {
                            LOG_RUNTIME("Different sizes of tensors!");
                        } else {
                            setTensorValue(m_value, value->m_value);
                        }
                    }
                }
                m_var_is_init = true;
                return;
            } else if ((is_none_type() || is_string_type()) && value->is_string_type()) {

                switch (m_var_type_current) {
                    case ObjType::None:
                    case ObjType::StrChar:
                        SetValue_(value->m_str);
                        return;
                    case ObjType::StrWide:
                        SetValue_(value->m_wstr);
                        return;
                }

            } else if (is_none_type() || isObjectType(m_var_type_current)) {
                std::string old_name = m_var_name;
                clear_();
                value->CloneDataTo(*this);
                value->ClonePropTo(*this);
                m_var_name.swap(old_name);
                m_var_is_init = true;

                return;
            } else if ((is_none_type() || m_var_type_current == ObjType::Pointer) && value->m_var_type_current == ObjType::Pointer) {
                //@todo Check tree type !!!

                std::string old_name = m_var_name;
                value->CloneDataTo(*this);
                value->ClonePropTo(*this);
                m_var_name.swap(old_name);
                m_var_is_init = true;

                return;
            } else if ((is_none_type() || m_var_type_current == ObjType::Function) && value->is_function()) {
                //@todo Check function type args !!!

                std::string old_name = m_var_name;
                value->CloneDataTo(*this);
                value->ClonePropTo(*this);
                m_var_name.swap(old_name);
                m_var_is_init = true;

                return;
            } else if ((is_none_type() || m_var_type_current == ObjType::Function || m_var_type_current == ObjType::EVAL_FUNCTION) && (value->m_var_type_current == ObjType::BLOCK || value->m_var_type_current == ObjType::BLOCK_TRY)) {
                //@todo Check function type args !!!

                //            std::string old_name = m_var_name;
                //            TermPtr save_proto = m_func_proto;
                //            TermPtr save_block = m_block_source;
                //            ObjType save_type = m_var_type_current;
                //            value->CloneDataTo(*this);
                //            value->ClonePropTo(*this);
                //            m_var_name.swap(old_name);
                //            m_var_is_init = true;
                //            *const_cast<TermPtr *> (&m_func_proto) = save_proto;
                m_block_source = value->m_block_source;
                //            m_var_type_current = save_type;

                return;
            }

            LOG_RUNTIME("Set value type '%s' not implemented!", newlang::toString(m_var_type_current));
        }

        static std::string format(std::string format, Obj *args);

        void clear_() override {

            Variable::clear_();
            clear_(true);
        }

        void clear_(bool clear_iterator_name) {

            m_str.clear();
            m_wstr.clear();
            m_var_type_current = ObjType::None;

            m_class_parents.clear();
            m_var_is_init = false;
            m_value.reset();
            m_fraction.reset();
            //        m_var = std::monostate();
            //        m_value.reset(); //????????????????
            //        m_items.clear();
        }


        bool CallAll(const char *func_name, ObjPtr &arg_in, ObjPtr &result, ObjPtr object = nullptr, size_t limit = 0); // ?
        bool CallOnce(ObjPtr &arg_in, ObjPtr &result, ObjPtr object = nullptr); // !


        static ObjPtr CreateFunc(Context *ctx, TermPtr proto, ObjType type, const std::string var_name = "");
        static ObjPtr CreateFunc(std::string proto, FunctionType *func_addr, ObjType type = ObjType::Function);

        ObjPtr ConvertToArgs(Obj *args, bool check_valid, Context *ctx) const {
            ObjPtr result = Clone();
            result->ConvertToArgs_(args, check_valid, ctx);

            return result;
        }

        void CheckArgsValid() const;
        bool CheckArgs() const;

        inline const TermPtr Proto() {

            return m_func_proto;
        }

    protected:

        void ConvertToArgs_(Obj *args, bool check_valid, Context *ctx = nullptr); // Обновить параметры для вызова функции или элементы у словаря при создании копии

    public:

        ObjType m_var_type_current; ///< Текущий тип значения объекта
        ObjType m_var_type_fixed; ///< Максимальный размер для арифметических типов, который задается разработчиком
        bool m_var_is_init; ///< Содержит ли объект корректное значение ???

        //    struct FuncInfo {
        //        const TermPtr m_func_proto;
        //        std::string m_func_mangle_name;
        //        void *m_func_ptr;
        //        ffi_abi m_func_abi;
        //    };
        //    std::variant<std::monostate, torch::Tensor, FuncInfo, std::string, std::wstring, std::string_view, std::wstring_view> m_var;


        std::string m_var_name; ///< Имя переменной, в которой хранится объект

        ObjPtr m_dimensions; ///< Размерности для ObjType::Type
        std::string m_class_name; ///< Имя класса объекта (у базовых типов отсуствует)
        std::vector<ObjPtr> m_class_parents; ///< Родительские классы (типы)

        std::string m_namespace;
        std::string m_str;
        std::wstring m_wstr;
        mutable std::pair<std::string, Variable<ObjPtr>::Type> m_str_pair; //< Для доступа к отдельным символам строк

        const TermPtr m_func_proto;
        std::string m_func_mangle_name;
        std::string m_module_name;
        void *m_func_ptr;
        ffi_abi m_func_abi;
        torch::Tensor m_value;
        std::shared_ptr<Fraction> m_fraction;

        TermPtr m_block_source;
        bool m_check_args; //< Проверять аргументы на корректность (для всех видов функций) @ref MakeArgs

        //    Context *m_ctx;


        //    SCOPE(protected) :
        bool m_is_const; //< Признак константы (по умолчанию изменения разрешено)
        bool m_is_reference; //< Признак ссылки на объект (mutable)
        size_t m_ref_count;


    };

} // namespace newlang

std::ostream & operator<<(std::ostream &out, newlang::Obj & var);
std::ostream & operator<<(std::ostream &out, newlang::ObjPtr var);


#endif // INCLUDED_NEWLANG_OBJECT_
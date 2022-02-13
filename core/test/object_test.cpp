#ifdef UNITTEST

#include "core/pch.h"
#include "core/fraction.h"

#include <core/warning_push.h>
#include <gtest/gtest.h>
#include <core/warning_pop.h>

#include <core/term.h>
#include <core/context.h>
#include <core/object.h>
#include <core/newlang.h>
#include <core/builtin.h>


using namespace std;
using namespace newlang;

TEST(ObjTest, Empty) {
    Object var;
    ASSERT_TRUE(var.empty());
    ASSERT_EQ(ObjType::None, var.getType());
}

TEST(ObjTest, Value) {
    Object var;

    var.SetValue_(0);
    ASSERT_EQ(ObjType::Bool, var.getType());

    var.SetValue_(1);
    ASSERT_EQ(ObjType::Bool, var.getType());

    var.SetValue_(1.0);
    ASSERT_EQ(ObjType::Double, var.getType());

    var.SetValue_(true);
    ASSERT_EQ(ObjType::Bool, var.getType());

    var.SetValue_(2);
    ASSERT_EQ(ObjType::Char, var.getType());

    var.SetValue_(-100);
    ASSERT_EQ(ObjType::Char, var.getType());

    var.SetValue_(1000);
    ASSERT_EQ(ObjType::Short, var.getType());

    var.SetValue_(100000);
    ASSERT_EQ(ObjType::Int, var.getType());

    var.SetValue_(10000000000);
    ASSERT_EQ(ObjType::Long, var.getType());

    var.SetValue_(2.0);
    ASSERT_EQ(ObjType::Double, var.getType());

    var.SetValue_(false);
    ASSERT_EQ(ObjType::Bool, var.getType());
}

TEST(ObjTest, String) {
    Object var;

    var.SetValue_(std::string("test"));
    ASSERT_EQ(ObjType::StrChar, var.getType()) << toString(var.getType());

    Object str1;
    str1.SetValue_(L"Test str");
    ASSERT_EQ(ObjType::StrWide, str1.getType()) << toString(str1.getType());

    Object str2;
    str2.SetValue_(std::string("test2"));
    ASSERT_EQ(ObjType::StrChar, str2.getType()) << toString(str2.getType());
}

TEST(ObjTest, Dict) {

    Context ctx(RunTime::Init());

    Object var(ObjType::Dict);
    ASSERT_TRUE(var.empty());
    EXPECT_THROW(
            var[0],
            std::out_of_range
            );
    ASSERT_EQ(0, var.size());
    ObjPtr var2 = ctx.Eval("()");

    var.push_back(Object::CreateString("Test1"));
    ASSERT_EQ(1, var.size());
    var.push_back(Object::CreateString("Test3"));
    ASSERT_EQ(2, var.size());
    var.insert(1, Object::CreateValue(2), "2");
    var.insert(0, Object::CreateValue(0), "0");
    ASSERT_EQ(4, var.size());

    ASSERT_TRUE(var[0]->op_accurate(Object::CreateValue(0)));
    ASSERT_TRUE(var[1]->op_accurate(Object::CreateString("Test1")));
    ASSERT_TRUE(var[2]->op_accurate(Object::CreateValue(2)));
    ASSERT_TRUE(var[3]->op_accurate(Object::CreateString(L"Test3")));

    var2 = ctx.Eval("(0, \"Test1\", 2, 'Test3')");

    ASSERT_TRUE((*var2)[0]->op_accurate(Object::CreateValue(0)));
    ASSERT_TRUE((*var2)[1]->op_accurate(Object::CreateString("Test1")));
    ASSERT_TRUE((*var2)[2]->op_accurate(Object::CreateValue(2)));
    ASSERT_TRUE((*var2)[3]->op_accurate(Object::CreateString(L"Test3")));

    ObjPtr var3 = ctx.Eval("(0, \"Test1\", 2, 'Test3',)");

    ASSERT_TRUE((*var3)[0]->op_accurate(Object::CreateValue(0)));
    ASSERT_TRUE((*var3)[1]->op_accurate(Object::CreateString("Test1")));
    ASSERT_TRUE((*var3)[2]->op_accurate(Object::CreateValue(2)));
    ASSERT_TRUE((*var3)[3]->op_accurate(Object::CreateString(L"Test3")));
}

TEST(ObjTest, AsMap) {

    ObjPtr map = Object::CreateType(ObjType::Dict);
    ASSERT_TRUE(map->empty());
    EXPECT_THROW(
            (*map)["test1"],
            std::out_of_range
            );
    ASSERT_EQ(0, map->size());
    ObjPtr temp = Object::CreateString("Test");
    map->push_back(temp, "test1");
    map->push_back(Object::CreateValue(100), "test2");
    ASSERT_EQ(2, map->size());
    ASSERT_STREQ((*map)["test1"]->toString().c_str(), temp->toString().c_str()) << temp->toString().c_str();

    ASSERT_TRUE((*map)["test2"]);
    ObjPtr temp100 = Object::CreateValue(100);
    ASSERT_TRUE(map->exist(temp100, true));

    ObjPtr test2 = (*map)["test2"];
    ASSERT_TRUE(test2);
    ASSERT_TRUE(test2);
    ASSERT_STREQ("100", test2->toString().c_str());
}

TEST(ObjTest, Eq) {

    ObjPtr var_int = Object::CreateValue(100);
    ObjPtr var_num = Object::CreateValue(100.0);
    ObjPtr var_str = Object::CreateString(L"STRING");
    ObjPtr var_bool = Object::CreateBool(true);
    ObjPtr var_empty = Object::CreateNone();

    ASSERT_EQ(var_int->m_var_type_current, ObjType::Char) << (int) var_int->getType();
    ASSERT_EQ(var_num->m_var_type_current, ObjType::Double) << (int) var_num->getType();
    ASSERT_EQ(var_str->m_var_type_current, ObjType::StrWide) << (int) var_str->getType();
    ASSERT_EQ(var_bool->m_var_type_current, ObjType::Bool) << (int) var_bool->getType();
    ASSERT_EQ(var_empty->m_var_type_current, ObjType::None) << (int) var_empty->getType();
    ASSERT_TRUE(var_empty->empty()) << (int) var_empty->getType();
    ASSERT_FALSE(var_int->empty()) << (int) var_int->getType();


    ASSERT_TRUE(var_int->op_accurate(Object::CreateValue(100)));
    ASSERT_FALSE(var_int->op_accurate(Object::CreateValue(111)));

    ASSERT_TRUE(var_num->op_equal(Object::CreateValue(100.0)));
    ASSERT_FALSE(var_num->op_equal(Object::CreateValue(100.0001)));
    ASSERT_TRUE(var_num->op_accurate(Object::CreateValue(100.0)));
    ASSERT_FALSE(var_num->op_accurate(Object::CreateValue(100.0001)));

    ASSERT_TRUE(var_int->op_equal(Object::CreateValue(100.0)));
    ASSERT_TRUE(var_int->op_accurate(Object::CreateValue(100.0)));
    ASSERT_FALSE(var_int->op_equal(Object::CreateValue(100.1)));
    ASSERT_FALSE(var_int->op_accurate(Object::CreateValue(100.1)));


    ObjPtr var_int2 = Object::CreateValue(101);
    ObjPtr var_num2 = Object::CreateValue(100.1);


    ASSERT_TRUE(var_int->op_accurate(var_int));
    ASSERT_FALSE(var_int->op_accurate(var_int2));

    ASSERT_TRUE(var_num->op_accurate(var_num));
    ASSERT_FALSE(var_num->op_accurate(var_num2));

    ASSERT_TRUE(var_int->op_equal(var_num));

    ASSERT_FALSE(var_int->op_equal(var_num2));
    ASSERT_FALSE(var_num2->op_equal(var_int));

    ObjPtr var_bool2 = Object::CreateBool(false);

    ASSERT_TRUE(var_bool->op_accurate(var_bool));
    ASSERT_FALSE(var_bool->op_accurate(var_bool2));

    ObjPtr var_str2 = Object::CreateString("STRING2");

    ASSERT_TRUE(var_str->op_accurate(var_str));
    ASSERT_TRUE(var_str->op_accurate(Object::CreateString("STRING")));
    ASSERT_FALSE(var_str->op_accurate(var_str2));


    ObjPtr var_empty2 = Object::CreateNone();

    ASSERT_TRUE(var_empty->op_accurate(var_empty));
    ASSERT_TRUE(var_empty->op_accurate(var_empty2));
}

TEST(ObjTest, Exist) {

    Object var_array(ObjType::Dict);

    var_array.push_back(Object::CreateString("item1"));
    var_array.push_back(Object::CreateString("item2"));

    ObjPtr item = Object::CreateString("item1");
    ASSERT_TRUE(var_array.exist(item, true));
    item->SetValue_("item2");
    ASSERT_TRUE(var_array.exist(item, true));
    item->SetValue_("item");
    ASSERT_FALSE(var_array.exist(item, true));


    Object var_map(ObjType::Dict);

    var_map.push_back(Object::CreateString("MAP_VALUE1"), "map1");
    var_map.push_back(Object::CreateString("MAP_VALUE2"), "map2");

    ASSERT_TRUE(var_map[std::string("map1")]);
    ASSERT_TRUE(var_map["map2"]);
    ASSERT_EQ(var_map.select("map"), var_map.end());

}

TEST(ObjTest, Intersec) {

    Object var_array(ObjType::Dict);
    Object var_array2(ObjType::Dict);

    var_array.push_back(Object::CreateString("item1"));
    var_array.push_back(Object::CreateString("item2"));

    ObjPtr result = var_array.op_bit_and(var_array2, true);
    ASSERT_TRUE(result->empty());

    var_array2.push_back(Object::CreateString("item3"));

    result = var_array.op_bit_and(var_array2, true);
    ASSERT_TRUE(result->empty());

    var_array2.push_back(Object::CreateString("item1"));
    result = var_array.op_bit_and(var_array2, true);
    ASSERT_FALSE(result->empty());
    ASSERT_EQ(1, result->size());

    var_array2.push_back(Object::CreateString("item2"));
    result = var_array.op_bit_and(var_array2, true);
    ASSERT_FALSE(result->empty());
    ASSERT_EQ(2, result->size());
    //    
    //    result = Op:intersec(var_array, var_array2);
    //    ASSERT_TRUE(Var::isEmpty(result));
    //
    //    
    //    ASSERT_TRUE(var_array.Exist(std::string("item1")));
    //
    //    ASSERT_TRUE(var_array.Exist(std::string("item1")));
    //    ASSERT_TRUE(var_array.Exist(std::string("item2")));
    //    ASSERT_FALSE(var_array.Exist(std::string("item")));
    //
    //
    //    Var var_map(Var::Type::MAP);
    //
    //    var_map.Set("map1", "MAP1");
    //    var_map.Set("map2", "MAP2");
    //
    //    ASSERT_TRUE(var_map.Exist(std::string("map1")));
    //    ASSERT_TRUE(var_map.Exist(std::string("map2")));
    //    ASSERT_FALSE(var_map.Exist(std::string("map")));




    //    Var var_object(Var::Type::OBJECT);
    //    Var var_list(Var::Type::LIST);
    //    
    //    var_list.Set("map1", "MAP1");
    //    var_map.Set("map2", "MAP2");
    //    
    //    ASSERT_TRUE(var_map.Exist("map1"));
    //    ASSERT_TRUE(var_map.Exist("map2"));
    //    ASSERT_FALSE(var_map.Exist("map"));
}

TEST(ObjTest, Print) {

    Context ctx(RunTime::Init());

    ObjPtr var_int = Object::CreateValue(100);
    ObjPtr var_num = Object::CreateValue(100.123);
    ObjPtr var_str = Object::CreateString(L"STRCHAR");
    ObjPtr var_strbyte = Object::CreateString("STRBYTE");
    ObjPtr var_bool = Object::CreateBool(true);
    ObjPtr var_empty = Object::CreateNone();

    ASSERT_STREQ("100", var_int->toString().c_str()) << var_int;
    ASSERT_STREQ("100.123", var_num->toString().c_str()) << var_num;
    ASSERT_STREQ("\"STRCHAR\"", var_str->toString().c_str()) << var_str;
    ASSERT_STREQ("'STRBYTE'", var_strbyte->toString().c_str()) << var_strbyte;
    ASSERT_STREQ("1", var_bool->toString().c_str()) << var_bool;
    ASSERT_STREQ("_", var_empty->toString().c_str()) << var_empty;

    ObjPtr var_dict = Object::CreateType(ObjType::Dict);

    ASSERT_STREQ("(,)", var_dict->toString().c_str()) << var_dict;

    var_dict->m_var_name = "dict";
    ASSERT_STREQ("dict=(,)", var_dict->toString().c_str()) << var_dict;

    var_dict->push_back(Object::CreateString(L"item1"));
    ASSERT_STREQ("dict=(\"item1\",)", var_dict->toString().c_str()) << var_dict;
    var_dict->push_back(var_int);
    var_dict->push_back((*var_bool)(nullptr));
    ASSERT_STREQ("dict=(\"item1\", 100, 1,)", var_dict->toString().c_str()) << var_dict;


    ObjPtr var_array = Object::CreateDict(); //CreateTensor({1});

    ASSERT_STREQ("(,)", var_array->toString().c_str()) << var_array;

    var_array->m_var_name = "array";
    ASSERT_STREQ("array=(,)", var_array->toString().c_str()) << var_array;

    var_array->push_back(Object::CreateString("item1"));
    ASSERT_STREQ("array=('item1',)", var_array->toString().c_str()) << var_array;
    var_array->push_back((*var_int.get())(nullptr));
    var_array->push_back((*var_bool.get())(nullptr));
    ASSERT_STREQ("array=('item1', 100, 1,)", var_array->toString().c_str()) << var_array;


    ObjPtr obj_empty = Object::CreateType(ObjType::Class, "name");
    ASSERT_STREQ("name=()", obj_empty->toString().c_str()) << obj_empty;

    ObjPtr var_obj = Object::CreateFrom("obj", obj_empty);
    ASSERT_STREQ("obj=name()", var_obj->toString().c_str()) << var_obj;

    var_int->m_var_name = "int";
    var_obj->push_back((*var_int.get())(nullptr));
    ASSERT_STREQ("obj=name(int=100)", var_obj->toString().c_str()) << var_obj;

}

TEST(ObjTest, CreateFromInteger) {

    Context ctx(RunTime::Init());

    ObjPtr var = Context::CreateRVal(&ctx, Parser::ParseString("123"));
    ASSERT_TRUE(var);
    ASSERT_EQ(ObjType::Char, var->getType()) << toString(var->getType());
    ASSERT_EQ(123, var->GetValueAsInteger());

    ObjPtr var2 = ctx.Eval("123");
    ASSERT_TRUE(var2);
    ASSERT_EQ(ObjType::Char, var2->getType()) << toString(var2->getType());
    ASSERT_EQ(123, var2->GetValueAsInteger());

    var = Context::CreateRVal(&ctx, Parser::ParseString("-123"));
    ASSERT_TRUE(var);
    ASSERT_EQ(ObjType::Char, var->getType()) << toString(var->getType());
    ASSERT_EQ(-123, var->GetValueAsInteger());

    var2 = ctx.Eval("-123");
    ASSERT_TRUE(var2);
    ASSERT_EQ(ObjType::Char, var2->getType()) << toString(var2->getType());
    ASSERT_EQ(-123, var2->GetValueAsInteger());

    ASSERT_THROW(
            var = Context::CreateRVal(&ctx, Term::Create(TermID::INTEGER, "")),
            std::runtime_error);

    ASSERT_THROW(
            var = Context::CreateRVal(&ctx, Term::Create(TermID::INTEGER, "lkdfjsha")),
            std::runtime_error);

    ASSERT_THROW(
            var = Context::CreateRVal(&ctx, Term::Create(TermID::INTEGER, "123lkdfjsha")),
            std::runtime_error);
}

//TEST(ObjTest, CreateFromDecimal) {
//
//    Context ctx(RunTime::Init());
//
//    ObjPtr var = Context::CreateRVal(&ctx, Parser::ParseString("123:Decimal"));
//    ASSERT_TRUE(var);
//    ASSERT_EQ(ObjType::Decimal, var->getType()) << toString(var->getType());
//    ASSERT_EQ(123, var->GetValueAsInteger());
//
//    ObjPtr var2 = ctx.Eval("-123.56789:Decimal");
//    ASSERT_TRUE(var2);
//    ASSERT_EQ(ObjType::Decimal, var2->getType()) << toString(var2->getType());
//    ASSERT_EQ(-123.56789, var2->GetValueAsNumber());
//
//    var = Context::CreateRVal(&ctx, Parser::ParseString("-123:Decimal"));
//    ASSERT_TRUE(var);
//    ASSERT_EQ(ObjType::Decimal, var->getType()) << toString(var->getType());
//    ASSERT_EQ(-123, var->GetValueAsInteger());
//
//    var2 = ctx.Eval("-123:Decimal");
//    ASSERT_TRUE(var2);
//    ASSERT_EQ(ObjType::Decimal, var2->getType()) << toString(var2->getType());
//    ASSERT_EQ(-123, var2->GetValueAsInteger());
//
//}

TEST(ObjTest, CreateFromNumber) {

    Context ctx(RunTime::Init());

    ObjPtr var = Context::CreateRVal(&ctx, Parser::ParseString("123.123"));
    ASSERT_TRUE(var);
    ASSERT_EQ(ObjType::Double, var->getType());
    ASSERT_DOUBLE_EQ(123.123, var->GetValueAsNumber());

    ObjPtr var2 = ctx.Eval("123.123");
    ASSERT_TRUE(var2);
    ASSERT_EQ(ObjType::Double, var2->getType());
    ASSERT_DOUBLE_EQ(123.123, var2->GetValueAsNumber());

    var = Context::CreateRVal(&ctx, Parser::ParseString("-123.123"));
    ASSERT_TRUE(var);
    ASSERT_EQ(ObjType::Double, var->getType());
    ASSERT_DOUBLE_EQ(-123.123, var->GetValueAsNumber());

    var2 = ctx.Eval("-123.123");
    ASSERT_TRUE(var2);
    ASSERT_EQ(ObjType::Double, var2->getType());
    ASSERT_DOUBLE_EQ(-123.123, var2->GetValueAsNumber());

    ASSERT_THROW(
            var = Context::CreateRVal(&ctx, Term::Create(TermID::NUMBER, "")),
            std::runtime_error);

    ASSERT_THROW(
            var = Context::CreateRVal(&ctx, Term::Create(TermID::NUMBER, "lkdfjsha")),
            std::runtime_error);

    ASSERT_THROW(
            var = Context::CreateRVal(&ctx, Term::Create(TermID::NUMBER, "123lkdfjsha")),
            std::runtime_error);
}

TEST(ObjTest, CreateFromString) {

    Context ctx(RunTime::Init());

    ObjPtr var = Context::CreateRVal(&ctx, Parser::ParseString("\"123.123\""));
    ASSERT_TRUE(var);
    ASSERT_EQ(ObjType::StrWide, var->getType());
    ASSERT_STREQ("123.123", var->GetValueAsString().c_str());

    ObjPtr var2 = ctx.Eval("\"123.123\"");
    ASSERT_TRUE(var2);
    ASSERT_EQ(ObjType::StrWide, var2->getType());
    ASSERT_STREQ("123.123", var2->GetValueAsString().c_str());

    var = Context::CreateRVal(&ctx, Parser::ParseString("'строка'"));
    ASSERT_TRUE(var);
    ASSERT_EQ(ObjType::StrChar, var->getType());
    ASSERT_STREQ("строка", var->GetValueAsString().c_str());

    var2 = ctx.Eval("'строка'");
    ASSERT_TRUE(var2);
    ASSERT_EQ(ObjType::StrChar, var2->getType());
    ASSERT_STREQ("строка", var2->GetValueAsString().c_str());
}

TEST(Args, All) {
    Context ctx(RunTime::Init());
    TermPtr ast;
    Parser p(ast);

    ASSERT_TRUE(p.Parse("test()"));
    Object local;
    Object proto1(&ctx, ast, false, local); // Функция с принимаемыми аргументами (нет аргументов)
    ASSERT_EQ(0, proto1.size());
    //    ASSERT_FALSE(proto1.m_is_ellipsis);


    Object arg_999(Object::CreateValue(999));
    EXPECT_EQ(ObjType::Short, arg_999[0]->getType()) << torch::toString(toTorchType(arg_999[0]->getType()));

    Object arg_empty_named(Object::Arg());
    ASSERT_EQ(ObjType::None, arg_empty_named[0]->getType());

    Object arg_123_named(Object::Arg(123, "named"));
    EXPECT_EQ(ObjType::Char, arg_123_named[0]->getType()) << torch::toString(toTorchType(arg_123_named[0]->getType()));

    ObjPtr arg_999_123_named = Object::CreateDict();
    ASSERT_EQ(0, arg_999_123_named->size());
    *arg_999_123_named += arg_empty_named;
    ASSERT_EQ(1, arg_999_123_named->size());
    *arg_999_123_named += arg_123_named;
    ASSERT_EQ(2, arg_999_123_named->size());

    ASSERT_THROW(
            proto1.ConvertToArgs(arg_999, true, nullptr), // Прототип не принимает позиционных аргументов
            std::invalid_argument);

    ASSERT_THROW(
            proto1.ConvertToArgs(arg_empty_named, true, nullptr), // Прототип не принимает именованных аргументов
            std::invalid_argument);


    ASSERT_TRUE(p.Parse("test(arg1)"));
    const Object proto2(&ctx, ast, false, local);
    ASSERT_EQ(1, proto2.size());
    //    ASSERT_FALSE(proto2.m_is_ellipsis);
    ASSERT_STREQ("arg1", proto2.name(0).c_str());
    ASSERT_EQ(nullptr, proto2.at(0).second);

    ObjPtr o_arg_999 = proto2.ConvertToArgs(arg_999, true, nullptr);
    ASSERT_TRUE((*o_arg_999)[0]);
    ASSERT_STREQ("999", (*o_arg_999)[0]->toString().c_str());

    //    proto2[0].reset(); // Иначе типы гурментов буду отличаться
    ObjPtr o_arg_empty_named = proto2.ConvertToArgs(arg_empty_named, false, nullptr);
    ASSERT_TRUE((*o_arg_empty_named)[0]);
    ASSERT_STREQ("_", (*o_arg_empty_named)[0]->toString().c_str());

    ASSERT_THROW(
            proto2.ConvertToArgs(arg_123_named, true, nullptr), // Имя аругмента отличается
            std::invalid_argument);



    // Нормальный вызов
    ASSERT_TRUE(p.Parse("test(empty=_, ...) := {}"));
    const Object proto3(&ctx, ast->Left(), false, local);
    ASSERT_EQ(2, proto3.size());
    //    ASSERT_TRUE(proto3.m_is_ellipsis);
    ASSERT_STREQ("empty", proto3.name(0).c_str());
    ASSERT_TRUE(proto3.at(0).second);
    ASSERT_EQ(ObjType::None, proto3.at(0).second->getType());
    ASSERT_STREQ("...", proto3.name(1).c_str());
    ASSERT_FALSE(proto3.at(1).second);

    ObjPtr proto3_arg = proto3.ConvertToArgs(arg_999, true, nullptr);
    ASSERT_EQ(1, proto3_arg->size());
    ASSERT_TRUE((*proto3_arg)[0]);
    ASSERT_STREQ("999", (*proto3_arg)[0]->toString().c_str());
    ASSERT_STREQ("empty", proto3_arg->name(0).c_str());

    // Дополнительный аргумент
    Object arg_extra(Object::CreateValue(999), Object::Arg(123, "named"));

    ASSERT_EQ(2, arg_extra.size());
    EXPECT_EQ(ObjType::Short, arg_extra[0]->getType()) << torch::toString(toTorchType(arg_extra[0]->getType()));
    EXPECT_EQ(ObjType::Char, arg_extra[1]->getType()) << torch::toString(toTorchType(arg_extra[1]->getType()));


    ObjPtr proto3_extra = proto3.ConvertToArgs(arg_extra, true, nullptr);
    ASSERT_EQ(2, proto3_extra->size());
    ASSERT_STREQ("999", (*proto3_extra)[0]->toString().c_str());
    ASSERT_STREQ("empty", proto3_extra->name(0).c_str());
    ASSERT_STREQ("123", (*proto3_extra)[1]->toString().c_str());
    ASSERT_STREQ("named", proto3_extra->name(1).c_str());


    // Аргумент по умолчанию
    ASSERT_TRUE(p.Parse("test(num=123) := {}"));
    Object proto123(&ctx, ast->Left(), false, local);
    ASSERT_EQ(1, proto123.size());
    //    ASSERT_FALSE(proto123.m_is_ellipsis);
    ASSERT_STREQ("num", proto123.name(0).c_str());
    ASSERT_TRUE(proto123[0]);
    ASSERT_STREQ("123", proto123[0]->toString().c_str());


    // Изменен порядок
    ASSERT_TRUE(p.Parse("test(arg1, str=\"string\") := {}"));
    Object proto_str(&ctx, ast->Left(), false, local);
    ASSERT_EQ(2, proto_str.size());
    //    ASSERT_FALSE(proto_str.m_is_ellipsis);
    ASSERT_STREQ("arg1", proto_str.at(0).first.c_str());
    ASSERT_EQ(nullptr, proto_str[0]);

    Object arg_str(Object::Arg(L"СТРОКА", "str"), Object::Arg(555, "arg1"));

    ObjPtr proto_str_arg = proto_str.ConvertToArgs(arg_str, true, nullptr);
    ASSERT_STREQ("arg1", proto_str_arg->at(0).first.c_str());
    ASSERT_TRUE(proto_str_arg->at(0).second);
    ASSERT_STREQ("555", (*proto_str_arg)[0]->toString().c_str());
    ASSERT_STREQ("str", proto_str_arg->at(1).first.c_str());
    ASSERT_TRUE((*proto_str_arg)[1]);
    ASSERT_STREQ("\"СТРОКА\"", (*proto_str_arg)[1]->toString().c_str());


    ASSERT_TRUE(p.Parse("test(arg1, ...) := {}"));
    Object proto_any(&ctx, ast->Left(), false, local);
    //    ASSERT_TRUE(proto_any.m_is_ellipsis);
    ASSERT_EQ(2, proto_any.size());
    ASSERT_STREQ("arg1", proto_any.at(0).first.c_str());
    ASSERT_EQ(nullptr, proto_any.at(0).second);

    ObjPtr any = proto_any.ConvertToArgs(arg_str, true, nullptr);
    ASSERT_EQ(2, any->size());
    ASSERT_STREQ("arg1", any->at(0).first.c_str());
    ASSERT_TRUE(any->at(0).second);
    ASSERT_STREQ("555", (*any)[0]->toString().c_str());
    ASSERT_STREQ("str", any->at(1).first.c_str());
    ASSERT_TRUE(any->at(1).second);
    ASSERT_STREQ("\"СТРОКА\"", (*any)[1]->toString().c_str());

    //
    ASSERT_TRUE(p.Parse("min(arg, ...) := {}"));
    Object min_proto(&ctx, ast->Left(), false, local);
    Object min_args(Object::Arg(200), Object::Arg(100), Object::Arg(300)); // min(200,100,300)
    ObjPtr min_arg = min_proto.ConvertToArgs(min_args, true, nullptr);

    ASSERT_EQ(3, min_arg->size());
    ASSERT_STREQ("200", (*min_arg)[0]->toString().c_str());
    ASSERT_STREQ("100", (*min_arg)[1]->toString().c_str());
    ASSERT_STREQ("300", (*min_arg)[2]->toString().c_str());

    ASSERT_TRUE(p.Parse("min(200, 100, 300)"));
    Object args_term(&ctx, ast, true, local);
    ASSERT_STREQ("200", args_term[0]->toString().c_str());
    ASSERT_STREQ("100", args_term[1]->toString().c_str());
    ASSERT_STREQ("300", args_term[2]->toString().c_str());
}

TEST(Types, FromLimit) {

    std::map<int64_t, ObjType> IntTypes = {
        {0, ObjType::Bool},
        {1, ObjType::Bool},
        {2, ObjType::Char},
        {127, ObjType::Char},
        //        {255, ObjType::Char},
        {256, ObjType::Short},
        {-1, ObjType::Char},
        {-200, ObjType::Short},
        {66000, ObjType::Int},
        {-33000, ObjType::Int},
        {5000000000, ObjType::Long},
    };

    for (auto elem : IntTypes) {
        ASSERT_EQ(elem.second, typeFromLimit(elem.first)) << elem.first << " " << toString(elem.second);
    }

    ASSERT_EQ(ObjType::Double, typeFromLimit(1.0));
    ASSERT_EQ(ObjType::Float, typeFromLimit(0.0));

}

TEST(Types, Convert) {

    /*
     * - Встроеные функции преобразования простых типов данных
     * - Передача аргументов функци по ссылкам
     */

    RuntimePtr opts = RunTime::Init();
    Context ctx(opts);


    EXPECT_TRUE(dlsym(nullptr, "_ZN7newlang4CharEPKNS_7ContextERKNS_6ObjectE"));
    EXPECT_TRUE(dlsym(nullptr, "_ZN7newlang6Short_EPNS_7ContextERNS_6ObjectE"));

    EXPECT_TRUE(opts->GetProcAddress(MangaledFunc("Char").c_str()));
    EXPECT_TRUE(opts->GetProcAddress(MangaledFunc("Char_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Bool").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Bool_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Char").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Char_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Short").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Short_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Int").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Int_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Long").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Long_").c_str()));

    //    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Half").c_str()));
    //    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Half_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Float").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Float_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Double").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Double_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("ComplexFloat").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("ComplexFloat_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("ComplexDouble").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("ComplexDouble_").c_str()));

    //    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("BFloat16").c_str()));
    //    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("BFloat16_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("StrChar").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("StrChar_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("StrWide").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("StrWide_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Dict").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Dict_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Class").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Class_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("None").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("None_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Integer").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Integer_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Number").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Number_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Complex").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("Complex_").c_str()));

    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("String").c_str()));
    ASSERT_TRUE(opts->GetProcAddress(MangaledFunc("String_").c_str()));


    ObjPtr byte = Object::CreateValue(1, ObjType::Bool);
    ASSERT_EQ(ObjType::Bool, byte->m_var_type_current) << toString(byte->m_var_type_current);

    ASSERT_TRUE(ctx.CallByName("Char", Object::Arg(byte)));
    ASSERT_EQ(ObjType::Char, ctx.CallByName("Char", Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    ASSERT_EQ(ObjType::Bool, byte->m_var_type_current) << toString(byte->m_var_type_current);

    ASSERT_EQ(ObjType::Char, newlang_Char(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    //    ASSERT_STREQ("1", newlang_Char(nullptr, Object::Arg(byte))->toString().c_str());

    ASSERT_EQ(ObjType::Short, newlang_Short(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    ASSERT_EQ(ObjType::Int, newlang_Int(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    ASSERT_EQ(ObjType::Long, newlang_Long(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    //    ASSERT_EQ(ObjType::Half, newlang_Half(nullptr, Object::Arg(byte))->m_var_type) << toString(byte->m_var_type);
    ASSERT_EQ(ObjType::Float, newlang_Float(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    ASSERT_EQ(ObjType::Double, newlang_Double(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);

    ASSERT_EQ(ObjType::Long, newlang_Integer(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);
    ASSERT_EQ(ObjType::Float, newlang_Number(nullptr, Object::Arg(byte))->m_var_type_current) << toString(byte->m_var_type_current);

    //    ASSERT_EQ(ObjType::Byte, newlang_Byte_(nullptr, Object::Arg(byte))->m_var_type) << toString(byte->m_var_type);
    //    ASSERT_EQ(ObjType::Byte, byte->m_var_type) << toString(byte->m_var_type);


    //    ASSERT_EQ(ObjType::Byte, ctx.CallByName("Byte_", Object::Arg(byte))->m_var_type) << toString(byte->m_var_type);
    //    ASSERT_EQ(ObjType::Byte, byte->m_var_type) << toString(byte->m_var_type);

    // @todo Для работы функций mutable функций требуется передача по ссылкам !!!!!!!!!!!!1

    //    ASSERT_EQ(ObjType::Char, ctx.CallByName("Char_", Object::Arg(byte))->m_var_type) << toString(byte->m_var_type);
    //    ASSERT_EQ(ObjType::Char, byte->m_var_type) << toString(byte->m_var_type);
    //
    //    ASSERT_EQ(ObjType::Double, ctx.CallByName("Double_", Object::Arg(byte))->m_var_type) << toString(byte->m_var_type);
    //    ASSERT_EQ(ObjType::Double, byte->m_var_type) << toString(byte->m_var_type);

    //    Object::CreateFromType();

}

/*
 * - создание и инициализация тензоров
 * - использование диапазонов при инициализации значений у словарей и тензоров
 * - встроенные функции и операторы у тензоров ?
 * - встроеные функции преобразования составных типов данных !
 * - использование диапазонов при индексации значений (срезы)
 * - использование многоточия при индексации значений у тензоров (для словарей не имеет значения, т.к. размерность только одна)
 */

TEST(ObjTest, Tensor) {

    RuntimePtr opts = RunTime::Init();
    Context ctx(opts);

    TermPtr term = Parser::ParseString("var:Int[2,3]");

    ObjPtr t1 = Context::CreateLVal(&ctx, term);
    ASSERT_EQ(ObjType::Int, t1->m_var_type_current);
    ASSERT_EQ(ObjType::Int, t1->m_var_type_fixed);
    ASSERT_FALSE(t1->m_var_is_init);

    ASSERT_EQ(2, t1->m_value.dim());
    ASSERT_EQ(2, t1->m_value.size(0));
    ASSERT_EQ(3, t1->m_value.size(1));
}

#endif // UNITTEST
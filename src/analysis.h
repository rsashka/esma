#pragma once
#ifndef INCLUDED_ANALISIS_CPP_
#define INCLUDED_ANALISIS_CPP_

#include "pch.h"

#include <sys/time.h>

#include <compiler.h>

#include "term.h"
#include "builtin.h"


#include <types.h>
#include <object.h>
#include <context.h>
#include <parser.h>


namespace newlang {

    class AstAnalysis {
    protected:

        AstAnalysis(const AstAnalysis&) = delete;
        const AstAnalysis& operator=(const AstAnalysis&) = delete;
    public:

        RunTime &m_rt;
        Diag * m_diag;

        AstAnalysis(RunTime &rt, Diag* diag) : m_rt(rt), m_diag(diag) {

        }

        /*
         * При раскрытии области видимости объектов и проверки корректности их имен,
         * нужно одновремно регистрировать глобальные и локальные переменные, 
         * так как nameloockup выполняется последовательно и если создавать переменные позже, 
         * то обращение может произойти к неправильным переменным 
         * (либо будет неправильное перекрытие, либо ошибка, т.к. переменная еще не создана).
         * 
         * А если регистрировать не только глобальные, но и локальные объекты, 
         * тогда в функцию анализатора нужно передавать и объект - владалец локальных переменных.
         * Точнее он уже есть, это либо глоальныей корень для переменных и функций модуля, 
         * либо глобальные объекты - функции с собственными локальными переменными.
         * m_variables
         */
        bool Analyze(TermPtr &term, TermPtr &root);
        bool RecursiveAnalyzer(TermPtr &term, ScopeStack &stack);
        bool CreateOp_(TermPtr &term, ScopeStack &stack);
        bool CreateVar(TermPtr &var, TermPtr &value, ScopeStack &stack);
        bool AssignVar(TermPtr &var, TermPtr &value, ScopeStack &stack);
        bool Iterator_(TermPtr &term, ScopeStack &stack);

        TermPtr LockupName(TermPtr &term, ScopeStack &stack);
        bool CheckNative_(TermPtr &term);

        /**
         * Функция проверки наличия ошибок при анализе AST.
         * Прерывает работу анализатора при превышении лимита или при force=true
         * @param force - Если есть ошибки- завершить работу
         */
        bool CheckError(bool result);

        bool CheckName(TermPtr &term);
        bool CheckOp(TermPtr &term);
        bool CalcType(TermPtr &term);
        bool UpcastOpType(TermPtr &op);
        bool CheckType(TermPtr &left, const TermPtr right);
        bool CheckCall(TermPtr &proto, TermPtr &call, ScopeStack & stack);
        bool CheckCallArg(TermPtr &call, size_t arg_pos, ScopeStack & stack);
        bool CkeckRange_(TermPtr &term, ScopeStack & stack);
        bool CheckNative_(TermPtr &proto, TermPtr &term);

        bool CheckArgsType_(TermPtr proto, TermPtr value);
        bool CheckArgs_(TermPtr proto, TermPtr args);

        static bool CheckStrFormat(const std::string_view format, TermPtr args, int start);
        static bool CheckStrPrintf(const std::string_view format, TermPtr args, int start);
        TermPtr CalcSummaryType(const TermPtr &term, ScopeStack & stack);

    protected:

    public:

    };
}


#endif //INCLUDED_ANALISIS_CPP_

#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <iostream>
#include <string>
#include <algorithm>

using std::string;

#include "type.hpp"
#include "functionsymbol.hpp"
#include "typehelper.hpp"
#include "templateinfo.hpp"
#include "builtins.hpp"
#include "callinfo.hpp"


class CodeGen
{
	public:

		static void emit(string text);

		template <class T, class U>
		static void genCallCode(CallInfo call_info, vector<ExprNode*> params, const TemplateInfo& template_info, T genFunc, U genThis)
		{
			auto func = call_info.callee;

			bool is_method = func->isMethod();

			int is_meth = (is_method ? 1 : 0);

			auto function_info = func->getTypeInfo();
			
			size_t params_size = std::accumulate(std::begin(function_info.params_types) + is_meth, std::end(function_info.params_types), 0, [](size_t x, VariableType type) { return x += type.getSize(); });

			if ( is_method )
				params_size += GlobalConfig::int_size;

			for ( int i = static_cast<int>(function_info.params_types.size()) - 1; i >= is_meth; --i )
			{
				genParam(params[i - is_meth], function_info.params_types[i], call_info.conversions[i - is_meth], call_info.copy_constructors[i - is_meth], template_info);
			}

			if ( is_method )
			{
				genThis();
				emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
				emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
			}

			genFunc();
			emit("call rax");
			emit("add rsp, " + std::to_string(params_size));
		}

		static void genParam(ExprNode *param, VariableType desired_type, ConversionInfo conv_info, FunctionSymbol *copy_constr, const TemplateInfo& template_info)
		{
			param->gen(template_info);

			if ( conv_info.deref )
				emit("mov eax, [eax]");
			
			if ( conv_info.conversion )
				genConversion(conv_info.conversion);

			if ( copy_constr == nullptr )
			{
				emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
				emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
			}
			else
			{
				if ( copy_constr == BuiltIns::int_copy_constructor )
				{
					emit("mov rbx, [rax]");
					emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
					emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
				}
				else
				{
					genCopy(copy_constr, 0, desired_type);
					emit("sub rsp, " + std::to_string(desired_type.getSize())); 
				}
			}

			if ( conv_info.ref )
			{
				emit("mov [rsp - 800], rax");
				emit("lea rax, [rsp - 800]");
			}
		}

		static void pushOnStack(size_t size, int offset);
		static void genConversion(FunctionSymbol *conv);
		static void genCopy(FunctionSymbol *copy_constructor, int stack_offset, VariableType type);
};

#endif

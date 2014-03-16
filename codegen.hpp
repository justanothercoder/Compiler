#ifndef _CODEGEN_HPP_
#define _CODEGEN_HPP_

#include <iostream>
#include <string>
#include <algorithm>

using std::string;

#include "type.hpp"
#include "functionsymbol.hpp"
#include "referencetype.hpp"
#include "typehelper.hpp"
#include "templatestructsymbol.hpp"
#include "builtins.hpp"
#include "callinfo.hpp"

class CodeGen
{
	public:

		static void emit(string text);

		template <class T, class U>
		static void genCallCode(CallInfo call_info, const vector<ExprNode*>& params, const TemplateStructSymbol *template_sym, std::vector<ExprNode*> template_expr, T genFunc, U genThis)
		{
			auto func = call_info.callee;

			int params_size = 0;

			bool is_method = func->isMethod();

			int is_meth = (is_method ? 1 : 0);

			auto function_info = func->getTypeInfo();
			
			params_size = std::accumulate(std::begin(function_info.params_types) + is_meth, std::end(function_info.params_types), 0, [](int x, Type *type) { return x += type->getSize(); });

			if ( is_method )
				params_size += GlobalConfig::int_size;

			for ( int i = static_cast<int>(function_info.params_types.size()) - 1; i >= is_meth; --i )
			{
				params[i - is_meth]->gen(template_sym, template_expr);

				auto conv_info = call_info.conversions[i - is_meth];

				if ( conv_info.deref )
					emit("mov rax, [rax]");

				if ( conv_info.conversion )
					genConversion(conv_info.conversion);

				auto copy_constr = call_info.copy_constructors[i - is_meth];	

				if ( copy_constr == BuiltIns::int_copy_constructor )
				{
					emit("mov rbx, [rax]");
					emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
					emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
				}
				else
				{
					genCopy(copy_constr, 0, function_info.params_types[i]);
					emit("sub rsp, " + std::to_string(function_info.params_types[i]->getSize())); 
				}

				if ( conv_info.ref )
				{
					emit("mov [rsp - 800], rax");
					emit("lea rax, [rsp - 800]");
				}
/*
				auto param_type = resolved_function_type_info.params_types[i];

				if ( param_type->isReference() )
				{
					emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rax");
					emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
				}
				else
				{
					if ( params[i - is_meth]->getType() != param_type )
					{
						auto par_type = params[i - is_meth]->getType();
						
						if ( par_type->isReference() && TypeHelper::removeReference(par_type) == param_type )
							par_type = TypeHelper::removeReference(par_type);
						else
						{
							auto conv = TypeHelper::getConversion(par_type, param_type);

							genConversion(conv);
						}
					}

					auto copy_constr = TypeHelper::getCopyConstructor(param_type);	
					if ( copy_constr == BuiltIns::int_copy_constructor )
					{
						emit("mov rbx, [rax]");
						emit("mov [rsp - " + std::to_string(GlobalConfig::int_size) + "], rbx");
						emit("sub rsp, " + std::to_string(GlobalConfig::int_size));
					}
					else
					{
						genCopy(copy_constr, 0, param_type);
						emit("sub rsp, " + std::to_string(param_type->getSize())); 
					}
				}
*/
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

		static void pushOnStack(size_t size, int offset);
		static void genConversion(FunctionSymbol *conv);
		static void genCopy(FunctionSymbol *copy_constructor, int stack_offset, Type *type);
};

#endif

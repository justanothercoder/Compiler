#include "callablenode.hpp"
#include "functionaltype.hpp"
#include "noviableoverloaderror.hpp"
#include "functionsymbol.hpp"

    /*
void CallableNode::genCall(GenSSAVisitor& gen)
{
    if ( inline_info.function_body )
        genInlineCall(call_info.callee, inline_info, args(), thisArg());
    else
    {
        auto params_size = 0;
        
        const auto& params = node -> params();

        for ( auto param = params.rbegin(); param != params.rend(); ++param )
        {
            auto info = *(node -> callInfo().conversions.rbegin() + (param - params.rbegin()));
            genParam(param -> get(), info);
            params_size += info.desired_type -> sizeOf();
        }

        if ( node -> callInfo().callee -> isMethod() )
        {
            params_size += Comp::config().int_size;
            genParam(node -> caller(), ConversionInfo(nullptr, TypeFactory::getReference(node -> caller() -> getType().base())));
        }

        genCall(node -> callInfo().callee, params_size);
    }
}
    */

VariableType CallableNode::getType() const { return call_info.callee -> type().returnType(); }
bool CallableNode::isLeftValue() const { return getType().isReference(); }

const CallInfo& CallableNode::callInfo() const { return call_info; }
void CallableNode::callInfo(const CallInfo& call_info) { this -> call_info = call_info; }

const InlineInfo& CallableNode::inlineInfo() const { return inline_info; }
void CallableNode::inlineInfo(InlineInfo inline_info) { this -> inline_info = std::move(inline_info); }

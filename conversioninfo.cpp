#include "conversioninfo.hpp"

ConversionInfo::ConversionInfo(FunctionSymbol *conversion, bool pre_deref, bool post_ref) : conversion(conversion), pre_deref(pre_deref), post_ref(post_ref) { }

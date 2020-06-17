
#pragma once

#define _STR(a) #a
#define STR(a) _STR(a)

#define COMPILATION_MESSAGE(message_string) message(__FILE__ "(" STR(__LINE__) "): " #message_string)

#ifndef BooleanFlagIntersection
#define BooleanFlagIntersection(F,SF) ((BOOLEAN)(((F) & (SF)) != 0))
#endif

#ifndef BooleanFlagInclude
#define BooleanFlagInclude(F,SF) ((BOOLEAN)(((F) & (SF)) == (SF)))
#endif

#ifndef SetFlag
#define SetFlag(_F,_SF) ((_F) | (_SF))
#endif

#ifndef ClearFlag
#define ClearFlag(_F,_SF) ((_F) & ~(_SF))
#endif

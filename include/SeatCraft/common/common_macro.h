#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define UNUSED_PARAM(x) (void)(x)
#else
#define UNUSED_PARAM(x) (void)(x)
#endif

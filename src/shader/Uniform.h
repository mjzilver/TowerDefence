#include <cstdint>

#define UNIFORM_LIST                    \
    X(Model, "model")                   \
    X(Projection, "projection")         \
    X(Color, "color")                   \
    X(Bounds, "bounds")                 \
    X(TexCoordOffset, "texCoordOffset") \
    X(TexCoordScale, "texCoordScale")   \
    X(UseRecolor, "useRecolor")         \
    X(Recolor, "recolor")               \
    X(TextColor, "textColor")           \
    X(Text, "text")

enum class Uniform : uint8_t {
#define X(name, str) name,
    UNIFORM_LIST
#undef X
        COUNT
};

constexpr const char* UNIFORM_NAMES[] = {
#define X(name, str) str,
    UNIFORM_LIST
#undef X
};

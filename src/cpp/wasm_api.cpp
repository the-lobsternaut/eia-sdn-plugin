/**
 * eia-sdn-plugin WASM API
 *
 * Parses WTI/Brent crude, natural gas, inventories
 */

#include "eia/types.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;
#endif

#include <string>
#include <cstring>

// ============================================================================
// SDN Plugin ABI
// ============================================================================

#ifdef __EMSCRIPTEN__

extern "C" {

EMSCRIPTEN_KEEPALIVE
void* sdn_malloc(size_t size) {
    return malloc(size);
}

EMSCRIPTEN_KEEPALIVE
void sdn_free(void* ptr) {
    free(ptr);
}

EMSCRIPTEN_KEEPALIVE
int32_t parse(const char* input, size_t input_len,
              uint8_t* output, size_t output_len) {
    try {
        std::string text(input, input_len);
        auto ds = eia::parse_json(text);
        std::string fb = eia::to_flatbuffers(ds);
        if (fb.size() > output_len) return -2;
        std::memcpy(output, fb.data(), fb.size());
        return static_cast<int32_t>(fb.size());
    } catch (...) {
        return -1;
    }
}

EMSCRIPTEN_KEEPALIVE
int32_t convert(const uint8_t* input, size_t input_len,
                uint8_t* output, size_t output_len) {
    // TODO: implement format conversion
    (void)input; (void)input_len;
    (void)output; (void)output_len;
    return -1;
}

} // extern "C"

EMSCRIPTEN_BINDINGS(sdn_eia) {
    function("parseJson", &eia::parse_json);
    function("validate", &eia::validate);
}

#endif

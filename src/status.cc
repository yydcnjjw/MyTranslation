#include "status.h"
#include <assert.h>
#include <string.h>

namespace MyTranslation {

const char *Status::CopyState(const char *state) {
    uint32_t size;
    memcpy(&size, state, sizeof(size));
    char *result = new char[size + 5];
    memcpy(result, state, size + 5);
    return result;
}

Status::Status(Code code, const std::string &msg) {
    assert(code != S_OK);
    const uint32_t len = msg.size();
    char *result = new char[len + 5];
    memcpy(result, &len, sizeof(len));
    result[4] = static_cast<char>(code);
    memcpy(result + 5, msg.data(), len);
    state_ = result;
}

std::string Status::ToString() const {
    if (state_ == nullptr) {
        return "OK";
    } else {
        char tmp[30];
        const char *type;
        switch (code()) {
        case S_OK:
            type = "OK";
            break;
        case S_NETWORK_ERROR:
            type = "Network error: ";
            break;
        case S_JSON_PARSER_ERROR:
            type = "Json parse error: ";
            break;
        default:
            snprintf(tmp, sizeof(tmp),
                     "Unknown code(%d): ", static_cast<int>(Code()));
            type = tmp;
            break;
        }
        std::string result(type);
        uint32_t length;
        memcpy(&length, state_, sizeof(length));
        result.append(state_ + 5, length);
        return result;
    }
}
} // namespace Translation

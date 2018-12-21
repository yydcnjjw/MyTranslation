#ifndef STATUS_H
#define STATUS_H

#include <string>

namespace MyTranslation {
class Status {
  public:
    Status() noexcept : state_(nullptr) {}
    ~Status() { delete[] state_; }

    Status(const Status &rhs);
    Status &operator=(const Status &rhs);

    Status(Status &&rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }
    Status &operator=(Status &&rhs) noexcept;

    static Status OK() { return Status(); }
    static Status NetworkError(const std::string &msg) {
        return Status(S_NETWORK_ERROR, msg);
    }
    static Status JsonParsorError(const std::string &msg) {
        return Status(S_JSON_PARSER_ERROR, msg);
    }

    static Status ShortcutError(const std::string &msg) {
        return Status(S_SHORTCUT_ERROR, msg);
    }

    bool ok() const { return (code() == S_OK); }
    bool networkError() const { return (code() == S_NETWORK_ERROR); }
    bool jsonParsorError() const { return (code() == S_JSON_PARSER_ERROR); }
    bool shortcutError() const { return (code() == S_SHORTCUT_ERROR); }
    std::string ToString() const;

  private:
    const char *state_;
    enum Code {
        S_OK = 0,
        S_NETWORK_ERROR = 1,
        S_JSON_PARSER_ERROR = 2,
        S_SHORTCUT_ERROR = 3
    };
    Status(Code code, const std::string &msg);
    static const char *CopyState(const char *s);

    Code code() const {
        return (state_ == nullptr) ? S_OK : static_cast<Code>(state_[4]);
    }
};

inline Status::Status(const Status &rhs) {
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
}

inline Status &Status::operator=(const Status &rhs) {
    if (state_ != rhs.state_) {
        delete[] state_;
        state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    return *this;
}

inline Status &Status::operator=(Status &&rhs) noexcept {
    std::swap(state_, rhs.state_);
    return *this;
}

} // namespace MyTranslation

#endif /* STATUS_H */

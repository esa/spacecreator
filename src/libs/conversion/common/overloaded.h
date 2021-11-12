#pragma once

namespace conversion {

/**
 * @brief   Overloaded pattern for the std::visit
 */
template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

} // namespace conversion

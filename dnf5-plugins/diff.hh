#ifndef DNF5_DIFF_PLUGIN_DIFF_HPP
#define DNF5_DIFF_PLUGIN_DIFF_HPP

#include <dnf5/context.hpp>
#include <libdnf5-cli/session.hpp>

#include <string>
#include <vector>


namespace dnf5 {


class DiffCommand : public Command {
public:
    explicit DiffCommand(Context & context) : Command(context, "diff") {}
    void set_parent_command() override;
    void set_argument_parser() override;
    void configure() override;
    void run() override;

private:
    std::vector<std::string> positional_args{};
    std::unique_ptr<libdnf5::cli::session::BoolOption> test_switch{nullptr};
    libdnf5::OptionString * test_string_option{nullptr};

    const std::vector<std::string> changed_files(const std::string & package_name);
};


}  // namespace dnf5


#endif  // DNF5_DIFF_PLUGIN_DIFF_HPP

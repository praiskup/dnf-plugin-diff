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
    void register_subcommands() override;
    void pre_configure() override;
    void configure() override;
    void load_additional_packages() override;
    void run() override;
    void goal_resolved() override;

private:
    std::vector<std::string> positional_args{};
    std::unique_ptr<libdnf5::cli::session::BoolOption> test_switch{nullptr};
    libdnf5::OptionString * test_string_option{nullptr};
};


}  // namespace dnf5


#endif  // DNF5_DIFF_PLUGIN_DIFF_HPP

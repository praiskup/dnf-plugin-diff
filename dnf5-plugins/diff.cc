#include "diff.hh"

#include <libdnf5-cli/session.hpp>

#include <iostream>


namespace dnf5 {

using namespace libdnf5::cli;

void DiffCommand::set_parent_command() {
    // Set a parent command and optionally also the group the command belongs to.
    std::cout << "----- executing DiffCommand::set_parent_command() method -----" << std::endl;
    auto * arg_parser_parent_cmd = get_session().get_argument_parser().get_root_command();
    auto * arg_parser_this_cmd = get_argument_parser_command();
    arg_parser_parent_cmd->register_command(arg_parser_this_cmd);
}

void DiffCommand::set_argument_parser() {
    // Register command arguments to the argument parser
    std::cout << "----- executing DiffCommand::set_argument_parser() method -----" << std::endl;
    auto & ctx = get_context();
    auto & parser = ctx.get_argument_parser();
    auto & cmd = *get_argument_parser_command();

    // Set the description of the command (printed in the list of commands section of dnf5 --help)
    cmd.set_description("Example dnf5 plugin command diff");

    // Register positional arguments
    auto args = parser.add_new_positional_arg("args", ArgumentParser::PositionalArg::UNLIMITED, nullptr, nullptr);
    args->set_description("Positional arguments.");
    args->set_parse_hook_func(
        [this]([[maybe_unused]] ArgumentParser::PositionalArg * arg, int argc, const char * const argv[]) {
            for (int i = 0; i < argc; ++i) {
                positional_args.emplace_back(argv[i]);
            }
            return true;
        });
    cmd.register_positional_arg(args);

    // register the --test/-t switch (option without value)
    test_switch = std::make_unique<libdnf5::cli::session::BoolOption>(
          *this, "test", 't', "Test switch", false);

    // register the --test-string option with a value
    test_string_option =
        dynamic_cast<libdnf5::OptionString *>(parser.add_init_value(std::make_unique<libdnf5::OptionString>(nullptr)));
    auto test_string_arg = parser.add_new_named_arg("test_string");
    test_string_arg->set_long_name("test-string");
    test_string_arg->set_description("Test string option with a value");
    test_string_arg->set_has_value(true);
    test_string_arg->set_arg_value_help("<VALUE>");
    test_string_arg->link_value(test_string_option);
    cmd.register_named_arg(test_string_arg);

}

void DiffCommand::register_subcommands() {
    // If the plugin command have any subcommands, register them
    std::cout << "----- executing DiffCommand::register_subcommands() method -----" << std::endl;
}

void DiffCommand::pre_configure() {
    // Adjust the configuration.
    // This method is called after parsing the command line arguments, but before loading standard configuration files.
    std::cout << "----- executing DiffCommand::pre_configure() method -----" << std::endl;
}

void DiffCommand::configure() {
    // Adjust the configuration.
    // This method is called after loading the configuration files, before the repository metadata are downloaded.
    // Right place to specify which repositories should be loaded (here the system repo and all enabled repos)
    std::cout << "----- executing DiffCommand::configure() method -----" << std::endl;
    auto & context = get_context();
    context.set_load_system_repo(true);
    context.set_load_available_repos(Context::LoadAvailableRepos::ENABLED);
}

void DiffCommand::load_additional_packages() {
    // Load additional packages that are not in the repositories
    // E.g. `repoquery` commands uses it to load also packages from rpm files
    // passed as command line arguments.
    std::cout << "----- executing DiffCommand::load_additional_packages() method -----" << std::endl;
}

void DiffCommand::run() {
    // Main plugin command logic goes here.
    std::cout << "----- executing DiffCommand::run() method -----" << std::endl;
    // Prints values of argument passed to the command.
    std::cout << "positional arguments:" << std::endl;
    for (const auto & arg : positional_args) {
        std::cout << "    " << arg << std::endl;
    }
    std::cout << "--test switch value: " << test_switch->get_value() << std::endl;
    if (test_string_option->get_priority() == libdnf5::Option::Priority::EMPTY) {
        std::cout << "--test-string argument was not used" << std::endl;
    } else {
        std::cout << "--test-string value: " << test_string_option->get_value() << std::endl;
    }
    // If the goal does not exist, this call creates it
    auto goal = get_context().get_goal();
    // Install dnf5 (this should be no-op)
    goal->add_install("dnf5");
}

void DiffCommand::goal_resolved() {
    // Called immediately after the goal is resolved, but before the
    // resulting transaction is executed.
    std::cout << "----- executing DiffCommand::goal_resolved() method -----" << std::endl;
}

}  // namespace dnf5

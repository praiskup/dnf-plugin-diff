#include "diff.hh"

#include <libdnf5/rpm/package_query.hpp>
#include <libdnf5/repo/package_downloader.hpp>
#include <libdnf5-cli/session.hpp>
#include <boost/process.hpp>

#include <iostream>
#include <filesystem>

namespace bp = boost::process;

namespace dnf5 {


using namespace libdnf5::cli;

void DiffCommand::set_parent_command() {
    // register our command
    auto * arg_parser_parent_cmd = get_session().get_argument_parser().get_root_command();
    auto * diff_cmd = get_argument_parser_command();
    arg_parser_parent_cmd->register_command(diff_cmd);
}

void DiffCommand::set_argument_parser() {
    // Register command arguments to the argument parser
    auto & diff_cmd = *get_argument_parser_command();
    auto & parser = diff_cmd.get_argument_parser();

    diff_cmd.set_description("Show changes in installed packages as a diff");

    auto args = parser.add_new_positional_arg("PACKAGE [FILENAME]",
            ArgumentParser::PositionalArg::UNLIMITED, nullptr, nullptr);
    args->set_description("Specify package name you want to diff, and optionally a filename.");
    args->set_parse_hook_func(
        [this]([[maybe_unused]] ArgumentParser::PositionalArg * arg, int argc, const char * const argv[]) {
            for (int i = 0; i < argc; ++i) {
                positional_args.emplace_back(argv[i]);
            }
            return true;
        });
    diff_cmd.register_positional_arg(args);
}

void DiffCommand::configure() {
    auto & context = get_context();
    context.set_load_system_repo(true);
    context.set_load_available_repos(Context::LoadAvailableRepos::ENABLED);
}

const std::vector<std::string> DiffCommand::changed_files(const std::string & package_name) {
    bp::ipstream out;
    bp::child c("/usr/libexec/dnf-diff-changed-files",
                std::vector<std::string> {package_name},
                bp::std_out > out);
    std::vector<std::string> changed_files;
    std::string changed_file;
    while (std::getline(out, changed_file)) {
        changed_files.push_back(changed_file);
    }
    return changed_files;
}

void DiffCommand::run() {
    if (positional_args.size() < 1)
        throw std::runtime_error("Package name not specified");

    auto &package_name = positional_args[0];

    // load the installed Package metadata
    libdnf5::rpm::PackageQuery installed_pkg(get_context().get_base());
    installed_pkg.filter_installed();
    installed_pkg.filter_name(std::vector<std::string>{package_name.c_str()});

    if (installed_pkg.empty())
        throw std::runtime_error("Package not installed: " + package_name);

    if (installed_pkg.size() > 1)
        throw std::runtime_error("Multiple packages installed: " + package_name);

    // separate query for the online counterpart (has location)
    libdnf5::rpm::PackageQuery available_pkg(get_context().get_base());
    available_pkg.filter_available();

    for (const auto & pkg : installed_pkg) {
        available_pkg.filter_name(std::vector<std::string>{pkg.get_name()});
        available_pkg.filter_version(pkg.get_version());
        available_pkg.filter_release(pkg.get_release());
        available_pkg.filter_arch(pkg.get_arch());
        break;
    }

    auto const &diff_files = changed_files(package_name);
    if (diff_files.empty())
        throw std::runtime_error("No changes in '" + package_name + "'.");

    // download the RPM file
    libdnf5::repo::PackageDownloader downloader(get_context().get_base());
    const char * home = getenv("HOME");
    std::filesystem::path download_dir(home);
    download_dir /= ".cache/dnf-diff";
    std::string rpm_filename;
    for (const auto & pkg : available_pkg) {
        downloader.add(pkg, download_dir);
        std::filesystem::path loc = pkg.get_location();
        rpm_filename = loc.filename();
        break;  // we take the first occurrence
    }
    downloader.download();

    // extract the RPM file, and run diff against changed files
    for (auto const &filename : diff_files)
        bp::system("/usr/libexec/dnf-diff-rpm-filename", rpm_filename, filename);
}


}  // namespace dnf5

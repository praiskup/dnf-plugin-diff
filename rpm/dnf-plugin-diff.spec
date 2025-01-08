%global __python %__python3

Name:           dnf-plugin-diff
Version:        1.1
Release:        1%{?dist}
Summary:        Display local changes in files from installed RPM packages
BuildArch:      noarch

License:        GPLv2+
URL:            https://github.com/praiskup/%name
Source0:        https://github.com/praiskup/%name/archive/v%version/%name-%version.tar.gz

BuildRequires:  python3-devel
BuildRequires:  make

Requires:       cpio
Requires:       python3-dnf
Requires:       python3-dnf-plugins-core
Requires:       file

Provides:       dnf-command(diff) = %version


%description
DNF plugin to compare files from the original package contents with locally
available files and display differences in text files, such as configuration
changes or source code (for interpreted languages).


%prep
%setup -q


%build
%configure PYTHON=python3
%make_build


%install
%make_install


%files
%license COPYING
%doc README
%_libexecdir/dnf-diff-*
%python3_sitelib/dnf-plugins


%changelog
* Mon Sep 03 2018 Pavel Raiskup <praiskup@redhat.com>
- no changelog in upstream git

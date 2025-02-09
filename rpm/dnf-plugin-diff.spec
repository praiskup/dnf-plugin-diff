%if 0%{?fedora} >= 41
%bcond_without dnf5
%else
%bcond_with dnf5
%endif

%global __python %__python3

%global _desc \
DNF plugin to compare files from the original package contents with locally \
available files and display differences in text files, such as configuration \
changes or source code (for interpreted languages).

%global _summary Display local changes in files from installed RPM packages

Name:           dnf-plugin-diff
Version:        2.0
Release:        1%{?dist}
Summary:        %_summary

License:        GPL-2.0-or-later
URL:            https://github.com/praiskup/%name
Source0:        https://github.com/praiskup/%name/archive/v%version/%name-%version.tar.gz

BuildRequires:  python3-devel
BuildRequires:  /usr/bin/c++
BuildRequires:  make

Requires:       cpio
Requires:       python3-dnf
Requires:       python3-dnf-plugins-core
Requires:       file

Provides:       dnf-command(diff) = %version

%if %{with dnf5}
BuildRequires:  boost-devel
BuildRequires:  dnf5-devel
BuildRequires:  libdnf5-cli-devel
%else
BuildArch:      noarch
%endif

%description
%_desc


%if %{with dnf5}
%package -n dnf5-plugin-diff
Requires:       dnf5
Summary:        %_summary
Provides:       dnf5-command(diff) = %version

%description -n dnf5-plugin-diff
%_desc
%endif

%prep
%setup -q


%build
%if %{with dnf5}
%configure PYTHON=python3
%else
%configure PYTHON=python3 --disable-dnf5
%endif
%make_build


%install
%make_install


%files
%license COPYING
%doc README
%_libexecdir/dnf-diff-*
%python3_sitelib/dnf-plugins


%if %{with dnf5}
%files -n dnf5-plugin-diff
%license COPYING
%doc README
%_libexecdir/dnf-diff-*
%_libdir/dnf5/plugins/diff.so
%endif


%changelog
* Mon Sep 03 2018 Pavel Raiskup <praiskup@redhat.com>
- no changelog in upstream git

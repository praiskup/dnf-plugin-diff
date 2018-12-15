%global __python %__python3

Name:           dnf-plugin-diff
Version:        1.0
Release:        1%{?dist}
Summary:        Diff changes in packages
BuildArch: noarch

License:        GPLv2+
URL:            https://github.com/praiskup/%{name}
Source0:        https://github.com/praiskup/%{name}/archive/v%{version}/%{name}-%{version}.tar.gz

BuildRequires:  python3-devel

Requires:       cpio
Requires:       dnf
Requires:       file

Provides:       dnf-command(diff) = %version


%description
Attempt to diff packages against local changes.


%prep
%setup -q


%build
%configure PYTHON=python3
%make_build


%install
%make_install


%files
%doc
%_libexecdir/dnf-diff-*
%python3_sitelib/dnf-plugins


%changelog
* Mon Sep 03 2018 Pavel Raiskup <praiskup@redhat.com>
- no changelog in upstream git

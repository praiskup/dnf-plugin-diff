%global __python %__python3

Name:           dnf-plugin-diff
Version:        1.0
Release:        1%{?dist}
Summary:        show local changes in RPM packages
BuildArch:      noarch

License:        GPLv2+
URL:            https://github.com/praiskup/%{name}
Source0:        https://github.com/praiskup/%name/releases/download/v%version/%name-%version.tar.gz

BuildRequires:  python3-devel

Requires:       cpio
Requires:       dnf
Requires:       file

Provides:       dnf-command(diff) = %version


%description
Dnf plugin to diff the original package contents against the locally changed
files.


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
* Sat Dec 15 2018 Pavel Raiskup <praiskup@redhat.com> - 1.0-1
- initial Fedora packaging

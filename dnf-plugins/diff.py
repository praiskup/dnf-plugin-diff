# supplies the 'diff' command.
#
# Copyright (C) 2018  Red Hat, Inc.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.

from __future__ import print_function
import os
import dnf
import rpm
import subprocess

import logging
from dnfpluginscore import _, logger

@dnf.plugin.register_command
class DiffCommand(dnf.cli.Command):
    aliases = ("diff",)
    summary = "Do blah."
    usage = "ahoj"

    @staticmethod
    def set_argparser(parser):
        parser.add_argument('pkg')
        parser.add_argument('file', nargs='*')

    def configure(self):
        # per https://bugzilla.redhat.com/1358245
        self.cli.redirect_logger(stdout=logging.WARNING, stderr=logging.INFO)
        self.base.conf.destdir = os.path.join(
            os.path.expanduser("~"),
            '.cache',
            'dnf-diff',
        )

    def _resolve_local_package(self, name):
        self.base.fill_sack()
        subj = dnf.subject.Subject(name)
        q = subj.get_best_query(self.base.sack)
        q = q.available()
        q = q.latest()
        if len(q.run()) == 0:
            msg = _("No package %s available.") % (name)
            raise dnf.exceptions.PackageNotFoundError(msg)

        return list(q)


    def _diff_package(self, package, package_name, files):
        for fname in files:
            if not fname in package.files:
                # Should not happen?
                logger.error(_("file '{0}' not found in '{1}'".format(fname,
                    package_name)))
                continue

            rpm_file_name = '{name}-{version}-{release}.{arch}.rpm'.format(
                    name=package.name,
                    version=package.version,
                    release=package.release,
                    arch=package.arch
            )

            subprocess.call(['/usr/libexec/dnf-diff-rpm-filename',
                             rpm_file_name,
                             fname])

    def _list_of_changed_files(self, package):
        proc = subprocess.Popen(
            ['/usr/libexec/dnf-diff-changed-files', package],
            stdout=subprocess.PIPE,
        )
        lines = [x.decode('ascii').rstrip() for x in proc.stdout.readlines()]
        return lines

    def run(self):
        to_download = self._resolve_local_package(self.opts.pkg)
        self.base.download_packages(to_download)

        for pkg in to_download:
            check_pkg = '{0}.{1}'.format(pkg.name, pkg.arch)
            if self.opts.file:
                self._diff_package(pkg, check_pkg, self.opts.file)
            else:
                self._diff_package(pkg, check_pkg,
                                   self._list_of_changed_files(check_pkg))

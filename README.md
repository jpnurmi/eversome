# EverN9

Evernote client for Nokia N9

## Submodules

After cloning EverN9, you must init and fetch the submodules:

<pre>
$ git clone git@github.com:jpnurmi/EverN9.git
Cloning into 'EverN9'...
[...]
$ cd EverN9
$ git submodule init
Submodule 'src/3rdparty/evernote' (git://github.com/evernote/evernote-sdk-cpp.git) registered for path 'src/3rdparty/evernote'
Submodule 'src/3rdparty/thrift' (git://github.com/apache/thrift.git) registered for path 'src/3rdparty/thrift'
$ git submodule update
Cloning into 'src/3rdparty/evernote'...
[...]
Submodule path 'src/3rdparty/evernote': checked out [...]
Cloning into 'src/3rdparty/thrift'...
[...]
Submodule path 'src/3rdparty/thrift': checked out [...]
</pre>

## Boost installation

<pre>
$ ~/QtSDK/Madde/bin/mad-admin list

Targets:
harmattan_10.2011.34-1_rt1.0  (installable)
harmattan_10.2011.34-1_rt1.1  (installable)
harmattan_10.2011.34-1_rt1.2  (installed)

Runtimes:
dfl61-harmattan_1.2011.34-1  (installable)
dfl61-harmattan_2.2011.39-5  (installable)
dfl61-harmattan_3.2012.07-1  (installable)

$ ~/QtSDK/Madde/bin/mad-admin set harmattan_10.2011.34-1_rt1.2

$ ~/QtSDK/Madde/bin/mad-admin xdpkg -i path/to/EverN9/pkg/libboost1.42-dev_1.42.0-3maemo2+0m6_armel.deb
Package 'libboost1.42-dev' installed.
</pre>

## Known issues

QtSDK packaging problem on Mac OSX:
<pre>
23:27:02: Creating package file...
23:27:03: Package Creation: Running command '/Users/jpnurmi/QtSDK/Madde/bin/mad dpkg-buildpackage -nc -uc -us'.
[...]
dpkg-buildpackage: failure: write changes file: Permission denied
dpkg-genchanges: binary-only upload - not including any source code
Packaging Error: Command '/Users/jpnurmi/QtSDK/Madde/bin/mad dpkg-buildpackage -nc -uc -us' failed.Exit code: 13
Error while building/deploying project EverN9 (target: Harmattan)
When executing step 'Create Debian Package'
</pre>

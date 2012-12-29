# Eversome

A QtQuick based Evernote client

## Submodules

After cloning the repo, you must init and fetch the submodules:

<pre>
$ git clone git@github.com:jpnurmi/eversome.git
Cloning into 'eversome'...
[...]
$ cd eversome
$ git submodule init
Submodule 'src/3rdparty/edam' (git://github.com/evernote/evernote-sdk-cpp.git) registered for path 'src/3rdparty/edam'
Submodule 'src/3rdparty/thrift' (git://github.com/apache/thrift.git) registered for path 'src/3rdparty/thrift'
$ git submodule update
Cloning into 'src/3rdparty/edam'...
[...]
Submodule path 'src/3rdparty/edam': checked out [...]
Cloning into 'src/3rdparty/thrift'...
[...]
Submodule path 'src/3rdparty/thrift': checked out [...]
</pre>

## Boost installation

Building Eversome requires Boost installed in the development environment:

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

$ ~/QtSDK/Madde/bin/mad-admin xdpkg -i path/to/eversome/pkg/libboost1.42-dev_1.42.0-3maemo2+0m6_armel.deb
Package 'libboost1.42-dev' installed.
</pre>

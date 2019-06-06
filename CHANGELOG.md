# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [Unreleased]
- Fetch webconfig data from cloud, process and apply config settings
- Added Device.X_RDK_WebConfig.ConfigFile.[i]. dynamic table
- Avoid repeated factory reset notification when cmc is 512
- Removed "Device.TR069Notify" parameter from component caching
- Removed turn on notification for 'Device.NotifyComponent.X_RDKCENTRAL-COM_Connected-Client' parameter when mesh/plume or cujo is enabled
- Dbus system ready check is added before registering system ready callback
- Notification on system ready is added
- Removed pthread_detach() on main thread
- Updated to use nanomsg nng compatibility
- CMC, CID values are logged during SET
- CMC set logic for factory-reset notification is changed
- ReplaceTable crashes are fixed
- Removed CM agent dependency in ETHWAN case
- Fixed config json override issue observed in field
- Integrated WebPA Client Support in RDKB RPI and EMU 
- Code optimization, added NULL checks in libpd.c
- Added abort in configure if thread support isn't found 
- Fixed yocto integration related build errors
- Synced with latest code from CcspWebpaAdapter repository
- Integration tests for get values, set values, get attributes and set attributes are added
- Fixed set values crash for large parameter name/value
- Added setter functions for extern variables
- Added Fedora equivalent name for libexpat-dev.
- Added wrapper function for logging using cimplog library
- Added CcspWebpaAdapter code from gerrit repo
- Build fixes
    - openssl version mismatch 
    - log length >4MB
- External dependencies dbus & ccsp-commom-library are added
- switched to nanomsg version 1.1.4, also libparodus with nanomsg 1.1.4

## [0.0.1] - 2017-06-15
### Added
- Initial creation

[Unreleased]: https://github.com/Comcast/parodus2ccsp/compare/953ff88c5ad415ca486fe06164e794efa8021c5d...HEAD

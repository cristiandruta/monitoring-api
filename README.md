# monitoring-api

> API clients to showcase the RESTful API of the monitoring framework


## Summary

This module includes some tools, an API specification, and a demo application
that shows the interaction with the monitoring database used in DreamCloud. For
more information about the API specification in general, please have a look
into the attached documentation.


## Prerequisites

The monitoring API requires first a running server and database. In order to
install these requirements, please checkout the associated [monitoring server][server], first. Please note that the installation and setup steps mentioned below assume
that you are running a current Linux as operating system. We have tested the
monitoring API with Ubuntu 15.10.

Before you can proceed, please clone the repository:

```bash
git clone git://github.com/excess-project/monitoring-api.git
```


### Dependencies

This project requires the following dependencies to be installed:

| Component         | Homepage                    | Version   |
|------------------ |---------------------------  |---------  |
| curl              | http://curl.haxx.se/        | >= 7.37   |


To ease the process of setting up a development environment, we provide a basic
script that downloads all dependencies, installs them locally in the project
directory, and then performs some clean-up operations. Thus, compiling the
monitoring API can be performed in a sandbox without affecting your current
operating system.

Executing the following script

```bash
./setup.sh
```

results in a new directory named `bin`, which holds the required dependencies
for compiling the project.


## Installation

This section assumes that you've successfully installed all required dependencies as described in the previous paragraphs.

```bash
$ make
$ make install
```


## Project Structure

The compiled resources are found within the `dist` folder. Created resources
include:

- mf_client
- mf_update
- mf_create_workflow
- mf_new_experiment



## Acknowledgment

This project is realized through [EXCESS][excess]. EXCESS is funded by the EU 7th
Framework Programme (FP7/2013-2016) under grant agreement number 611183. We are
also collaborating with the European project [DreamCloud][dreamcloud].


## Contributing
Find a bug? Have a feature request?
Please [create](https://github.com/excess-project/monitoring-api/website/issues) an issue.


## Main Contributors

**Dennis Hoppe, HLRS**
+ [github/hopped](https://github.com/hopped)


## Release History

| Date        | Version | Comment          |
| ----------- | ------- | ---------------- |
| 2016-04-20  | 16.2    | Public release.  |


## License
Copyright (C) 2016 University of Stuttgart

[Apache License v2](LICENSE).

[server]: https://github.com/excess-project/monitoring-server
[excess]: http://www.excess-project.eu
[dreamcloud]: http://www.dreamcloud-project.eu
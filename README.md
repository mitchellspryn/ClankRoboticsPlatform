# ClankRoboticsPlatform

This repo contains a framework for building a robot using the UDOO quad. See the blog post for more information.


The code that runs on the UDOO's ARM processor can be found in the `clank/` subdirectory, and can be built and run using

```
$ cd clank/
$ mkdir build
$ cd build
$ cmake .. && make
$ ./bin/StartTeleoperatedRobot
```

Unit tests can be run by building the project and running
```
$ ./bin/RunAllUnitTests.sh
```

The frontend is an eclipse project, and can be used to connect to the robot once the teleoprated daemon is running.

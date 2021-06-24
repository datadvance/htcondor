# This is a temporary fork of htcondor which contains some kluges to make it works.

# Build from source for Ubuntu 20.04

```
# Clone the project
git clone git@github.com:herclogon/htcondor.git

# Build docker htcondor build environment
cd htcondor/build/nmi-build/x86_64_Ubuntu20
# Switch experimental branch
git checkout V8_9_99
sudo docker build . --tag htcondor-build-env

# Run build environment
cd ../../../
sudo docker run -ti --rm  --mount type=bind,source=$(pwd),target=/htcondor htcondor-build-env bash
cd /htcondor/
./configure_uw 
make
make install

```

# HTCondor

[HTCondor](https://research.cs.wisc.edu/htcondor/) is a
[Distributed High Throughput Computing](https://en.wikipedia.org/wiki/High-throughput_computing)
system developed at the
[Center for High Throughput Computing](http://chtc.cs.wisc.edu/)
at the University of Wisconsin - Madison.  With it, users can divide large
computing workloads into jobs and submit them to an HTCondor scheduler,
which will run them on worker nodes managed by HTCondor.

Prebuilt binaries for Linux, Windows and Mac can be
[downloaded.](https://research.cs.wisc.edu/htcondor/downloads/)

# Documentation

The [HTCondor manual](https://research.cs.wisc.edu/htcondor/manual/) is a
comprehensive reference for users and administrators of HTCondor.

# Community

The CHTC maintains active [email lists](https://research.cs.wisc.edu/htcondor/mail-lists/)
where the HTCondor community asks and answers questions about the installation,
use, or tuning of HTCondor.  If you have a question, encounter a surprise about
HTCondor, or a potential bug, these public email lists are the first place to go.

We welcome github pull requests for code fixes or documentation improvements, but if
you have ideas for a big feature change, please talk with us first.

[HTCondor Week](https://research.cs.wisc.edu/htcondor/HTCondorWeek2020/index.html)
is our annual community meeting in Madison, WI, and we often have an annual
meeting in Europe as well. We encourage everyone with an interest in HTCondor
to join us.

[Materials from past meetings](https://research.cs.wisc.edu/htcondor/past_condor_weeks.html)
include talks from science and industry, plus useful tutorials.

[HTCondor Wiki](http://condor-wiki.cs.wisc.edu/index.cgi/wiki) contains FAQs,
bug tickets, and more.

# LICENSE

The HTCondor source code is licensed under the Apache-2.0 Open Source License.
See the [NOTICE.txt](NOTICE.txt) file for full details.

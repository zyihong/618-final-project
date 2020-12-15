## OpenMP on GPUs

Check our [Proposal](https://docs.google.com/document/d/1ODHGpVbE_Mi9GmfAOWPZITt7Ah6zEe7cmiYSFjAQUw0/edit?ts=5fa34986#)

Check our [Milestone](https://docs.google.com/document/d/1vBdi9YX8FKgrUa8br-xBzTggGMQcvoeiqRx9tHPTjq0/edit?ts=5fc40584#)

Check our [Result](https://drive.google.com/file/d/1p3meHNWvMPHHGIezobuvdbJeaRpnkInv/view?usp=sharing)

### SUMMARY

We implemented the grid solver （partial differential equation (PDE) on a (N + 2) * (N + 2) grid） using OpenMP on both CPU and GPU. We explored different techniques to optimize both versions and compared their performance. We demonstrated what we used and why those techniques works.  Besides, we also showed performance gains of each optimization and also the comparative performance of graph between the CPU OpenMP and the GPU OpenMP.

### BACKGROUND

### THE CHALLENGE

### RESOURCE

### GOALS AND DELIVERABLES

### PLATFORM CHOICE

We used C++ and OpenMP 4.5 for the coding and the machine we used was a Core2 i7-8850H 2.6GHz (6 cores and 12 threads) and a GTX 1050Ti Max-Q graphics card.

### SCHEDULE

|  | Status | Task | Assignee |
| --- | --- | :-- | --- |
| Week 1 - 4 | Finished | Figure out how to use the complier for OpenMP 4.5 ASAP, start an OpenMP implementation. | Together |
| | Finished | Complete an OpenMP CPU implementation that can reasonably scale. | Together |
| | Finished | Have a working OpenMP on GPU and profiling results. | Together |
| Week 5 1st half | Working | Implement the red-black coloring version. | Yihong Zou |
| | Working | Implement the 4D blocking layout version. | Jiahao Zhang |
| | | Comparing and testing the performance and tuning using OpenMP constructs to better utilize the GPU. | Together |
| Week 5 2nd half |  | Explore other OpenMP constructs and try to write more explicit GPU code to better control the blocking division and synchronization. If any efficiency is found in API calls listed by nvprof, combine the OpenMP offloading with CUDA kernels explicitly. | Together |
| Week 6 |  | Report the performance gain of each iteration. If time permits, explore the CUDA implementation. | Together |

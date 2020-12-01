## OpenMP on GPUs

Check our [Proposal](https://docs.google.com/document/d/1ODHGpVbE_Mi9GmfAOWPZITt7Ah6zEe7cmiYSFjAQUw0/edit?ts=5fa34986#)

### SUMMARY

### BACKGROUND

### THE CHALLENGE

### RESOURCE

### GOALS AND DELIVERABLES

### PLATFORM CHOICE

### SCHEDULE

|  | Status | Task | Assignee |
| --- | --- | :-- | --- |
| Week 1 - 4 | Finished | Figure out how to use the complier for OpenMP 4.5 ASAP, start an OpenMP implementation. | Together |
| | Finished | Complete an OpenMP CPU implementation that can reasonably scale. | Together |
| | Finished | Have a working OpenMP on GPU and profiling results. | Together |
| Week 5 1st half | Working | Implement the red-black coloring version. | Yihong Zou |
| | | Implement the 4D blocking layout version. | Jiahao Zhang |
| | | Comparing and testing the performance and tuning using OpenMP constructs to better utilize the GPU. | Together |
| Week 5 2nd half |  | Explore other OpenMP constructs and try to write more explicit GPU code to better control the blocking division and synchronization. If any efficiency is found in API calls listed by nvprof, combine the OpenMP offloading with CUDA kernels explicitly. | Together |
| Week 6 |  | Report the performance gain of each iteration. If time permits, explore the CUDA implementation. | Together |

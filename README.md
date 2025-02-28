**Overview**
The two versions of the code differ in broadcasting, scattering, and gathering the data among processes. The first code uses MPI_Bcast to send matrix2 from process 0 to all other processes, whereas the second code does this broadcasting manually using MPI_Send and MPI_Recv. Regarding matrix1 scattering, the first code uses MPI_Scatter to send subparts of matrix1 to all processes. The second code manually scatters with MPI_Send and MPI_Recv, with the help of memcpy to copy data into sub_matrix1 for process 0. For result gathering, the first code uses MPI_Gather to collect all the results in matrix3. On the other hand, the second code manually gathers results with MPI_Send and MPI_Recv, and uses memcpy to copy sub_matrix3 results for process 0 into matrix3.

**1.	Size of the Problem Table**
![image](https://github.com/user-attachments/assets/11989824-bb53-4010-91b8-71fa75175859)
![image](https://github.com/user-attachments/assets/16a5ebe5-46d4-448f-9c2d-e6ee72b388a5)

The table shows the running time (in milliseconds) of parallel matrix multiplication code for different matrix sizes. Larger matrices take longer to multiply in parallel, but the speedup compared to a sequential implementation increases. You can see each V1 and V2’s run times.

**2.	Speed Up**
![image](https://github.com/user-attachments/assets/276df84e-446d-4a45-9976-0ec678b234cd)
![image](https://github.com/user-attachments/assets/9ba1ddc6-d16a-4b78-be7c-d683f1efe19f)
![image](https://github.com/user-attachments/assets/938ab506-45d9-4beb-8121-e85ccef3f23e)
![image](https://github.com/user-attachments/assets/5f730ee6-23e7-4518-b1ed-b2ca1900b6f8)

The Speed Up analysis, taking into account the 16-core configuration of the system, provides valuable insights into parallel computing performance. Across various matrix multiplication operations, Speed Up generally improves as the number of threads increases from 1 to 16, indicating some level of scalability within the system's core capabilities. However, beyond 16 threads, Speed Up either plateaus or decreases, highlighting the system's limitation in handling additional threads efficiently due to core constraints. This trend is particularly evident in the decreasing Speed Up values for larger thread counts across V1 and V2. The impact of core limitations on scalability is further reflected in the Efficiency metrics, which also show a decline after 16 threads. This analysis underscores the importance of different techniques while handling broadcasting, scattering and gathering.

**3.	Efficiency**
![image](https://github.com/user-attachments/assets/5967c39c-75a8-4474-bd35-8e3e685c9a7a)
![image](https://github.com/user-attachments/assets/38fba7c4-28ab-4dea-90e7-b936141ce271)
![image](https://github.com/user-attachments/assets/bf08ee11-9ced-4d62-a973-e64145cf9ef4)
![image](https://github.com/user-attachments/assets/6a1a6519-1a7a-458e-a077-6011f1fad792)

The choice of broadcasting, scattering and gathering type in MPI doesn’t really effect the efficiency of parallel computations, particularly on systems with a limited number of cores like the 16-core setup in this analysis. In both versions we can see the dramatic decrease after 16 processes which is expected on our system. 

**4.  Scalability Analysis**

The scalability analysis based on Speed Up and Efficiency values for different matrix multiplication operations reveals insights into the performance of parallel computing on the system with 16 cores. As the number of threads increases from 1 to 16, Speed Up generally shows improvements across two versions, indicating some level of scalability. However, beyond 16 threads, the Speed Up values plateau or decrease, suggesting limited scalability due to the system's core constraints. This observation aligns with the Efficiency metrics, which show a noticeable decrease in efficiency after 16 threads across two versions. The decrease in efficiency beyond 16 threads underscores the impact of core limitations on the system's ability to handle increasing thread counts effectively. Overall, while there is some scalability up to 16 threads, further scalability is limited due to the system's 16-core configuration, highlighting the importance of the method while broadcasting, scattering and gathering process based on core constraints for better parallel computing performance.







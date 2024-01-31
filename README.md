# MFG

## Running Format

The source code of our paper are in the folder "src".

The source code of each algorithm has five parameters, corresponding to the algorithm name (e.g., **VFree**), the dataset name (e.g., **D5**), the two size constraints $\tau_U$ and $\tau_V$ (e.g., **5 4**), and the frequency constraint &lambda; (e.g., **3**).

* Running example for Algorithms **VFree** and **VFree-**

  Take **VFree** as an example, on dataset **D5** with parameters $\tau_U$ = **5**, $\tau_V$ = **4** and &lambda; = **3**

  > g++ -std=c++11 RecurrenceGroup.cpp -O3 -o VFree
  >
  > ./VFree VFree D5 5 4 3


* Running example for Algorithms **FilterV**, **FilterV-**, **FilterV-FR**, **FilterV-VM** and **BK-ALG+**

  Take **FilterV** as an example, on dataset **D5** with parameters $\tau_U$ = **5**, $\tau_V$ = **4** and &lambda; = **3**

  > g++ -std=c++11 main.cpp graph.cpp -O3 -o FilterV
  >
  > ./FilterV FilterV D5 5 4 3


## Datasets

We provide five datasets in the folder "sample_dataset" for testing, D2 to D6.

Since the download of the clinical database (i.e., D1) requires some private information, we provide its official link here: 
https://physionet.org/content/mimiciii/1.4

Considering the space limitations of GitHub, the remaining datasets can be found at: 
https://1drv.ms/u/s!Aut6uXLH_Qy_bEUkKlJKmCLKnTs

The details of each dataset are shown in the following table.

| Dataset     | &#124; U &#124; | &#124; V &#124; | &#124; $\mathcal{E}$ &#124; | $\mathcal{E}$ Type | &#124; $\mathcal{T}$ &#124; |
| ----------- | --------------- | --------------- | --------------------------- | ------------------ | --------------------------- |
| D1 (MI)     | 100,000         | 15,648          | 58,951                      | diagnose           | 25                          |
| D2 (Ip)     | 28,540          | 37,088          | 73,153                      | click              | 31                          |
| D3 (diq)    | 25,771          | 1,526           | 133,874                     | edit               | 12                          |
| D4 (vec)    | 33,587          | 2,282           | 339,722                     | edit               | 14                          |
| D5 (LK)     | 337,510         | 42,046          | 605,642                     | post               | 35                          |
| D6 (ben)    | 249,726         | 79,269          | 845,577                     | edit               | 17                          |
| D7 (Wut)    | 530,419         | 175,215         | 2,118,877                   | usage              | 39                          |
| D8 (Bti)    | 767,448         | 204,674         | 2,517,857                   | assign             | 22                          |
| D9 (AR)     | 1,230,916       | 2,146,058       | 5,754,118                   | rate               | 21                          |
| D10 (id)    | 2,183,495       | 125,482         | 7,890,901                   | edit               | 59                          |
| D11 (ar)    | 2,943,712       | 209,374         | 13,601,759                  | edit               | 57                          |
| D12 (nl)    | 3,800,350       | 220,848         | 28,294,026                  | edit               | 65                          |
| D13 (it)    | 4,857,109       | 343,861         | 41,146,957                  | edit               | 65                          |
| D14 (fr)    | 8,870,763       | 757,622         | 66,586,964                  | edit               | 66                          |
| D15 (de)    | 5,910,433       | 1,025,085       | 70,745,969                  | edit               | 67                          |

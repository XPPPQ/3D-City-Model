library(lidR)
library(tidyverse)

setwd("D:/R_work/UAGP/Processes/0417LOD1_Final/Tree")
rLAS <- readLAS("Tree.las")

chm_p2r_05 <- grid_canopy(rLAS, 0.5, p2r(subcircle = 0.2))
ttops_chm_p2r_05 <- find_trees(chm_p2r_05, lmf(5))

algo <- dalponte2016(chm_p2r_05, ttops_chm_p2r_05)
rLAS_Seg <- segment_trees(rLAS, algo) # segment point cloud
plot(rLAS_Seg, bg = "white", size = 4, color = "treeID") # visualize trees

crowns <- delineate_crowns(rLAS_Seg) # identify crowns
par(mar=rep(0,4))
plot(crowns) # visualize crowns

library(rgdal)
writeOGR(crowns, ".", "xyz_extent", driver="ESRI Shapefile")

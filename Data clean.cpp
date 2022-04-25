
#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <iostream>
using namespace std;

int
main(int argc, char** argv)
{
	
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("buildingini.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	} cout << "there are " << cloud->points.size() << " points before filtering." << endl;


	
	pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
	kdtree->setInputCloud(cloud);

	
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> clustering;
	clustering.setClusterTolerance(4.0);
	clustering.setMinClusterSize(1000);
	clustering.setMaxClusterSize(200000); 
	clustering.setSearchMethod(kdtree);
	clustering.setInputCloud(cloud);
	std::vector<pcl::PointIndices> clusters;
	clustering.extract(clusters);

	
	int currentClusterNum = 1;
	for (std::vector<pcl::PointIndices>::const_iterator i = clusters.begin(); i != clusters.end(); ++i)
	{
		
		pcl::PointCloud<pcl::PointXYZ>::Ptr cluster(new pcl::PointCloud<pcl::PointXYZ>);
		for (std::vector<int>::const_iterator point = i->indices.begin(); point != i->indices.end(); point++)
			cluster->points.push_back(cloud->points[*point]);
		cluster->width = cluster->points.size();
		cluster->height = 1;
		cluster->is_dense = true;

		
		if (cluster->points.size() <= 0)
			break;
		std::cout << "Cluster " << currentClusterNum << " has " << cluster->points.size() << " points." << std::endl;
		std::string fileName = "cluster" + std::to_string(currentClusterNum) + ".pcd";
		pcl::io::savePCDFileASCII(fileName, *cluster);

		currentClusterNum++;
	}
}
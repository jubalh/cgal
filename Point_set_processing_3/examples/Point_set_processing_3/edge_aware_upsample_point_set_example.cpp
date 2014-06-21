#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/edge_aware_upsample_point_set.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/IO/write_xyz_points.h>

#include <vector>
#include <fstream>

// types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;


// Point with normal vector stored in a std::pair.
typedef std::pair<Point, Vector> PointVectorPair;


int main(void)
{
  const std::string INPUT_FILENAME_WITHOUT_EXT = "data/before_upsample";
 
  // Reads a .xyz point set file in points[], *with normals*.
  std::vector<PointVectorPair> points;
  std::ifstream stream(INPUT_FILENAME_WITHOUT_EXT + ".xyz");

  if (!stream ||
      !CGAL::read_xyz_points_and_normals(stream,
                        std::back_inserter(points),
                        CGAL::First_of_pair_property_map<PointVectorPair>(),
                        CGAL::Second_of_pair_property_map<PointVectorPair>()))
  {
    std::cerr << "Error: cannot read file " 
      << INPUT_FILENAME_WITHOUT_EXT << ".xyz" << std::endl;
    return EXIT_FAILURE;
  }

  //Algorithm parameters
  const double sharpness_angle = 25;   // control sharpness of the result.
  const double edge_senstivity = 0;    // higher values will sample more points near the edges          
  const double neighbor_radius = 0.2;  // initial size of neighborhood.
  const unsigned int number_of_output_points = points.size() * 50;   

   //Run algorithm 
   CGAL::edge_aware_upsample_point_set(
            points.begin(), 
            points.end(), 
            std::back_inserter(points),
            CGAL::First_of_pair_property_map<PointVectorPair>(),
            CGAL::Second_of_pair_property_map<PointVectorPair>(),
            sharpness_angle, 
            edge_senstivity,
            neighbor_radius,
            number_of_output_points);

  // Saves point set.
  std::ofstream out(INPUT_FILENAME_WITHOUT_EXT + "_UPSAMPLED.xyz");  

  if (!out ||
     !CGAL::write_xyz_points_and_normals(
      out, points.begin(), points.end(), 
      CGAL::First_of_pair_property_map<PointVectorPair>(),
      CGAL::Second_of_pair_property_map<PointVectorPair>()))
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}





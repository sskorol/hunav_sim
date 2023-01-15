#include <QtWidgets>
#include <QBasicTimer>
#undef NO_ERROR

#include <memory>
#include <string>
#include <vector>

#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "nav2_msgs/action/follow_waypoints.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "rviz_common/panel.hpp"
#include "rviz_common/tool.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "visualization_msgs/msg/marker_array.hpp"
#include "nav2_util/geometry_utils.hpp"
#include "rviz_common/display_context.hpp"

#include "yaml-cpp/yaml.h"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "rclcpp/qos.hpp"

#include <rviz_common/message_filter_display.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

class QLineEdit;

namespace hunav_rviz2_panel
{

class ActorPanel: public rviz_common::Panel, public rclcpp::Node
{

Q_OBJECT
public:

  ActorPanel( QWidget* parent = 0 );
  ~ActorPanel();

  virtual void load(const rviz_common::Config& config );
  virtual void save(rviz_common::Config config ) const;

public Q_SLOTS:

  void setTopic( const QString& topic );
  
protected Q_SLOTS:

  void addAgent();
  void saveAgents();
  void updateTopic();
  int processMouseEvent(rviz_common::ViewportMouseEvent &event);
  void onInitialPose(double x, double y, double theta, QString frame);
  void onNewGoal(double x, double y, double theta, QString frame);
  void getNewGoal();
  void closeGoalsWindow();
  void setInitialPose();
  void closeInitialPoseWindow();
  int checkComboBox();
  int checkComboBoxSkin();
  void parseYaml();
  void randomRGB();
  visualization_msgs::msg::Marker createMarker(double point1_x, double point1_y, double ids, std::string marker_shape);
  visualization_msgs::msg::Marker createArrowMarker(double point1_x, double point1_y, double point2_x, double point2_y, double ids);
  void removeCurrentMarkers();
  void removeGoalsMarkers();
  void removeMarker(visualization_msgs::msg::Marker marker);
  void openFileExplorer();

public:

  // QT variables
  QLineEdit *actors;
  QLineEdit* agent_name;
  QLineEdit* coordinates;
  QLineEdit* coordinates1;
  QLineEdit* coordinates2;
  QLineEdit* num_goals_set;
  // The current name of the output topic.
  QString output_topic_;

  QWidget *window = nullptr;
  QWidget *window1 = nullptr;
  QWidget *window2 = nullptr;
  QComboBox *behavior_combobox;
  QComboBox *skin_combobox;

  QObject *initial_pose_connection;
  QObject *goals_connection;

  QMetaObject::Connection* conn_delete = new QMetaObject::Connection();

  QPushButton *save_button;
  QPushButton *goals_button;

  QVBoxLayout *topic_layout_init_pose;
  QCheckBox *checkbox;
  
  std::vector<YAML::Node> actors_info;
  std::vector<std::string> names;
  std::vector<std::string> point;
  
  // Program logic
  bool first_actor = true;
  bool initial_pose_set = false;
  int num_actors;
  int iterate_actors = 1;
  int goals_number = 1;
  int goals_to_remove = 0;
  int marker_id = 0;
  int agent_count = 1;

  // Positions for markers
  geometry_msgs::msg::PoseStamped initial_pose;
  std::vector<geometry_msgs::msg::PoseStamped> poses;
  geometry_msgs::msg::PoseStamped pose;
  geometry_msgs::msg::PoseStamped oldPose;
  geometry_msgs::msg::PoseStamped stored_pose;

  // Markers
  std::vector<visualization_msgs::msg::Marker> markers_array;
  std::vector<visualization_msgs::msg::Marker> arrows_markers_array;

  // Colors for Markers
  std::vector<double> rgb{255,0};
  double red;
  double green;
  double blue;

  // Publishers
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr initial_pose_publisher;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr goals_publisher;
  rclcpp::Node::SharedPtr client_node_;
  std::unique_ptr<visualization_msgs::msg::MarkerArray> marker_array = std::make_unique<visualization_msgs::msg::MarkerArray>();

  // Dir to store file
  std::string pkg_shared_tree_dir_;
  std::string dir;
  bool show_file_selector_once = true;
};

}


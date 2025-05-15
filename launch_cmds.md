Unpack tgz: tar -xvzf /path/to/yourfile.tgz

# SLAM RGDB:
## ORB

./run_tum_rgbd_slam -v /stella_vslam/data/voc/orb_vocab_voc.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_sitting_xyz/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_sitting_xyz_orb --map-db-out /stella_vslam/ws/maps/fr3_sitting_xyz_orb.msg --viewer none

./run_tum_rgbd_slam -v /stella_vslam/data/voc/orb_vocab_voc.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_xyz/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_walking_xyz_orb --map-db-out /stella_vslam/ws/maps/fr3_walking_xyz_orb.msg --viewer none

./run_tum_rgbd_slam -v /stella_vslam/data/voc/orb_vocab_voc.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_rpy/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_walking_rpy_orb --map-db-out /stella_vslam/ws/maps/fr3_walking_rpy_orb.msg --viewer none

./run_tum_rgbd_slam -v /stella_vslam/data/voc/orb_vocab_voc.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_static/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_walking_static_orb --map-db-out /stella_vslam/ws/maps/fr3_walking_static_orb.msg --viewer none

## SuperPoint

./run_tum_rgbd_slam -v /stella_vslam/data/voc/superpoint_vocab.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_sitting_xyz/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_superpoint.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_sitting_xyz_superpoint --map-db-out /stella_vslam/ws/maps/fr3_sitting_xyz_superpoint.msg --viewer none

./run_tum_rgbd_slam -v /stella_vslam/data/voc/superpoint_vocab.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_xyz/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_superpoint.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_walking_xyz_superpoint --map-db-out /stella_vslam/ws/maps/fr3_walking_xyz_superpoint.msg --viewer none

./run_tum_rgbd_slam -v /stella_vslam/data/voc/superpoint_vocab.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_rpy/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_superpoint.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_walking_rpy_superpoint --map-db-out /stella_vslam/ws/maps/fr3_walking_rpy_superpoint.msg --viewer none

./run_tum_rgbd_slam -v /stella_vslam/data/voc/superpoint_vocab.fbow -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_static/ -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_superpoint.yaml --no-sleep --auto-term --eval-log-dir /stella_vslam/ws/logs/fr3_walking_static_superpoint --map-db-out /stella_vslam/ws/maps/fr3_walking_static_superpoint.msg --viewer none

# Evaluation:
## ORB
python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_sitting_xyz.txt /stella_vslam/ws/logs/fr3_sitting_xyz_orb/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_sitting_xyz_orb --verbose 

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_walking_xyz.txt /stella_vslam/ws/logs/fr3_walking_xyz_orb/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_walking_xyz_orb --verbose 

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_walking_rpy.txt /stella_vslam/ws/logs/fr3_walking_rpy_orb/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_walking_rpy_orb --verbose 

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_walking_static.txt /stella_vslam/ws/logs/fr3_walking_static_orb/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_walking_static_orb --verbose 

## SuperPoint

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_sitting_xyz.txt /stella_vslam/ws/logs/fr3_sitting_xyz_superpoint/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_sitting_xyz_superpoint --verbose 

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_walking_xyz.txt /stella_vslam/ws/logs/fr3_walking_xyz_superpoint/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_walking_xyz_superpoint --verbose 

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_walking_rpy.txt /stella_vslam/ws/logs/fr3_walking_rpy_superpoint/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_walking_rpy_superpoint --verbose 

python3 /stella_vslam/ws/evaluation/evaluate_ate_scale.py /stella_vslam/ws/evaluation/ground_truth/TUM_rgbd/fr3_walking_static.txt /stella_vslam/ws/logs/fr3_walking_static_superpoint/frame_trajectory.txt --save_dir=/stella_vslam/ws/evaluation/results/fr3_walking_static_superpoint --verbose 
# SLAM RGDB:
## ORB + BFMatcher
./run_tum_rgbd_slam     -v /stella_vslam/data/voc/orb_vocab.fbow     -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_sitting_xyz/     -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml     --no-sleep     --auto-term  --eval-log-dir /ws/logs/fr3_sitting_xyz_orb  --map-db-out /ws/maps/fr3_sitting_xyz_orb.msg 

./run_tum_rgbd_slam     -v /stella_vslam/data/voc/orb_vocab.fbow     -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_xyz/     -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml     --no-sleep     --auto-term  --eval-log-dir /ws/logs/fr3_walking_xyz_orb  --map-db-out /ws/maps/fr3_walking_xyz_orb.msg 

./run_tum_rgbd_slam     -v /stella_vslam/data/voc/orb_vocab.fbow     -d /stella_vslam/data/datasets/TUM/rgbd_dataset_freiburg3_walking_rpy/     -c /stella_vslam/example/tum_rgbd/TUM_RGBD_rgbd_1.yaml     --no-sleep     --auto-term  --eval-log-dir /ws/logs/fr3_walking_rpy_orb  --map-db-out /ws/maps/fr3_walking_rpy_orb.msg 

## SuperPoint + BFMatcher

# Evaluation:
## ORB + BFMatcher
python3 evaluation/evaluate_ate_scale.py evaluation/ground_truth/TUM_rgbd/fr3_sitting_xyz.txt /ws/logs/fr3_sitting_xyz_orb/frame_trajectory.txt --save_dir=/ws/evaluation/results/fr3_sitting_xyz_orb --verbose 

python3 evaluation/evaluate_ate_scale.py evaluation/ground_truth/TUM_rgbd/fr3_walking_xyz.txt /ws/logs/fr3_walking_xyz_orb/frame_trajectory.txt --save_dir=/ws/evaluation/results/fr3_walking_xyz_orb --verbose 

python3 evaluation/evaluate_ate_scale.py evaluation/ground_truth/TUM_rgbd/fr3_walking_rpy.txt /ws/logs/fr3_walking_rpy_orb/frame_trajectory.txt --save_dir=/ws/evaluation/results/fr3_walking_rpy_orb --verbose 

## SuperPoint + BFMatcher
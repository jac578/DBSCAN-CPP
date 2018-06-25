

usage:

cd build/

cmake ..

make 

./bin/test_filelist --feat-list ../tests/feats_bin_v3/feat_filelist.txt --image-list ../tests/imgs_aligned_112x112/img_filelist.txt --save-root ./cluster_result/ --eps 0.5 --num_pts 2

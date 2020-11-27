{
    "targets":[
        {
            "target_name":"image",
            'conditions': [
                ['OS=="win"',{
                     "sources":[
                        './src_win/utils.cc',
                        './src_win/base64.cc',
                        './src_win/imageDirFunction.cc',
                        './src_win/imageBase64Function.cc',
                        './src_win/image.cc',
                    ],
                    'libraries': [
                        "D:/opencv/opencv/build/x64/vc15/lib/opencv_world440.lib",
                    ],
                    'include_dirs':[
                        'D:/opencv/opencv/build/include',
                        'D:/opencv/opencv/build/include/opencv2',
                        './src_win/'
                    ]
                }],
                ['OS=="linux"',{
                    "sources":[
                        './src_linux/utils.cc',
                        './src_linux/base64.cc',
                        './src_linux/imageDirFunction.cc',
                        './src_linux/imageBase64Function.cc',
                        './src_linux/image.cc',
                    ],
                    'libraries': [
                        '-lopencv_dnn',
                        '-lopencv_highgui',
                        '-lopencv_ml',
                        '-lopencv_objdetect',
                        '-lopencv_photo',
                        '-lopencv_stitching',
                        '-lopencv_features2d',
                        '-lopencv_flann',
                        '-lopencv_imgcodecs',
                        '-lopencv_imgproc',
                        '-lopencv_core',
                        '-lpthread',
                        '-lrt',
                        '-lm',
                        '-ldl',
                    ],
                    'include_dirs': [
                        '/usr/local/include/opencv4',
                        './src_linux/'
                    ],
                    'ldflags': [
                        '-L/usr/local/lib64',
                    ]
                }]
            ],
            
        }
    ]
}
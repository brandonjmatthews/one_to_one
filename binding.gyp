{
    "targets": [{
        "target_name": "OneToOne",
        "sources": [
            "src/one_to_one.mm",
            "src/one_to_one.cc",
        ],
        "include_dirs": ["<!(node -e \"require('nan')\")"],
        "conditions": [
            ['OS=="mac"', {
                'defines': ['__MACOSX_CORE__'],
                'link_settings': {
                    'libraries': [
                        '-framework CoreFoundation',
                        '-framework CoreImage',
                    ]
                },
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'CLANG_CXX_LIBRARY': 'libc++',
                    'ENABLE_MODULES': 'YES',
                    'OTHER_CFLAGS': [
                        '-ObjC++',
                        '-std=c++11',
                        '-fobjc-arc'
                    ],
                },
            }],
            ['OS=="win"', {}],
            ['OS=="linux"', {}],
        ]
    }]
}
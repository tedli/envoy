load("@base_pip3//:requirements.bzl", "requirement")
load("@rules_python//python:defs.bzl", "py_binary")
load(
    "//bazel:envoy_build_system.bzl",
    "envoy_package",
)

licenses(["notice"])  # Apache 2

envoy_package()

exports_files([
    "google-vrp/envoy-edge.yaml",
])

py_binary(
    name = "configgen",
    srcs = ["configgen.py"],
    data = glob([
        "*.yaml",
    ]),
    deps = [
        requirement("jinja2"),
    ],
)

filegroup(
    name = "configs",
    srcs = glob(
        [
            "**/*.yaml",
        ],
        exclude = [
            "using_deprecated_config.yaml",
            "**/*.template.yaml",
            "freebind/freebind.yaml",
            "envoy-tap-config.yaml",
            "envoy-demo.yaml",
        ],
    ) + select({
        "//bazel:apple": ["envoy-demo.yaml"],
        "//bazel:windows_x86_64": [],
        "//bazel:disable_admin_functionality": [],
        "//conditions:default": [
            "envoy-demo.yaml",
            "envoy-tap-config.yaml",
            "freebind/freebind.yaml",
        ],
    }),
)

genrule(
    name = "example_configs",
    srcs = [
        ":configs",
        "@envoy_examples//:configs",
        "@envoy_examples//:certs",
        "@envoy_examples//:lua",
        # TODO(phlax): re-enable once wasm example is fixed
        # "@envoy_examples//wasm-cc:configs",
        "//docs:configs",
        "//docs:proto_examples",
        "//test/config/integration/certs",
    ],
    outs = ["example_configs.tar"],
    cmd = (
        "$(location configgen.sh) $(location configgen) example_configs.tar $(@D) " +
        "$(locations :configs) " +
        "$(locations @envoy_examples//:configs) " +
        "$(locations @envoy_examples//:certs) " +
        "$(locations @envoy_examples//:lua) " +
        # "$(locations @envoy_examples//wasm-cc:configs) " +
        "$(locations //docs:configs) " +
        "$(locations //docs:proto_examples) " +
        "$(locations //test/config/integration/certs)"
    ),
    tools = [
        "configgen.sh",
        ":configgen",
    ],
)

genrule(
    name = "example_contrib_configs",
    srcs = [
        "//docs:contrib_configs",
        "//contrib:configs",
        "@envoy_examples//:contrib_configs",
        "@envoy_examples//:certs",
        "//test/config/integration/certs",
    ],
    outs = ["example_contrib_configs.tar"],
    cmd = (
        "$(location configgen.sh) NO_CONFIGGEN example_contrib_configs.tar $(@D) " +
        "$(locations //contrib:configs) " +
        "$(locations //docs:contrib_configs) " +
        "$(locations @envoy_examples//:contrib_configs) " +
        "$(locations @envoy_examples//:certs) " +
        "$(locations //test/config/integration/certs)"
    ),
    tools = [
        "configgen.sh",
        ":configgen",
    ],
)

py_binary(
    name = "example_configs_validation",
    srcs = ["example_configs_validation.py"],
    args = (
        "--descriptor_path=$(location @envoy_api//:v3_proto_set)",
        "$(locations :configs) ",
        "$(locations @envoy_examples//:configs) ",
        "$(locations //docs:configs) ",
    ),
    data = [
        ":configs",
        "//docs:configs",
        "@envoy_api//:v3_proto_set",
        "@envoy_examples//:configs",
    ],
    deps = [requirement("envoy.base.utils")],
)

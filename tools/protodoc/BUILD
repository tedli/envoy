load("@base_pip3//:requirements.bzl", "requirement")
load("@com_github_grpc_grpc//bazel:python_rules.bzl", "py_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")
load("//tools/base:envoy_python.bzl", "envoy_genjson", "envoy_jinja_env", "envoy_py_data", "envoy_pytool_binary", "envoy_pytool_library")
load("//tools/protodoc:protodoc.bzl", "protodoc_rule")
load("//tools/python:namespace.bzl", "envoy_py_namespace")

licenses(["notice"])  # Apache 2

envoy_py_namespace()

envoy_pytool_binary(
    name = "generate_empty",
    srcs = ["generate_empty.py"],
    visibility = ["//visibility:public"],
    deps = [
        ":jinja",
        ":protodoc",
    ],
)

proto_library(
    name = "manifest_proto",
    srcs = ["manifest.proto"],
    deps = [
        "@com_google_protobuf//:struct_proto",
    ],
)

py_proto_library(
    name = "manifest_py_pb2",
    deps = [":manifest_proto"],
)

py_proto_library(
    name = "validate_py_pb2",
    deps = ["@com_envoyproxy_protoc_gen_validate//validate:validate_proto"],
)

envoy_py_data(
    name = "protodoc_manifest_untyped",
    src = "//docs:protodoc_manifest.yaml",
)

envoy_pytool_binary(
    name = "manifest_to_json",
    srcs = ["manifest_to_json.py"],
    args = ["$(location @envoy_api//:v3_proto_set)"],
    data = ["@envoy_api//:v3_proto_set"],
    deps = [
        requirement("envoy.base.utils"),
        ":manifest_py_pb2",
        ":protodoc_manifest_untyped",
        "@com_google_protobuf//:protobuf_python",
    ],
)

genrule(
    name = "manifest",
    outs = ["manifest.json"],
    cmd = """$(location :manifest_to_json) $(location @envoy_api//:v3_proto_set) $@""",
    tools = [
        ":manifest_to_json",
        "@envoy_api//:v3_proto_set",
    ],
)

envoy_genjson(
    # This prepares the data for `protodoc`
    # As protodoc can be run many times while building the api docs, all
    # data is prepared, validated and normalized before being passed to
    # `protodoc`
    name = "data_srcs",
    srcs = [":manifest.json"],
    # manifest:
    #   validated protodoc manifest (from `docs/protodoc_manifest.yaml`)
    #   contains edge config examples.
    # extensions/contrib_extensions:
    #    extensions metadata (from `source/extensions/extensions_metdata.yaml`
    #    and `contrib/extensions_metadata.yaml`)
    # extension/contrib_extension_categories
    #    index of extensions_categories -> extension (taken from metadata)
    # extension_status_values:
    #    schema for possible extension status (eg wip/stable)
    # extension_security_postures:
    #    schema for possible security postures (eg un/trusted up/downstream)
    filter = """
    {manifest: .[0],
     extensions: .[1],
     contrib_extensions: .[2],
     extension_categories: (
         .[1] | reduce to_entries[] as $item ({};
             .[$item.value.categories[]] += [$item.key])),
     contrib_extension_categories: (
         .[2] | reduce to_entries[] as $item ({};
             .[$item.value.categories[]] += [$item.key])),
     extension_status_values: (
         reduce .[3].status_values[] as $value ({};
             .[$value.name] = $value.description)),
     extension_security_postures: (
         reduce .[3].security_postures[] as $posture ({};
             .[$posture.name] = $posture.description))}
    """,
    yaml_srcs = [
        "//source/extensions:extensions_metadata.yaml",
        "//contrib:extensions_metadata.yaml",
        "//tools/extensions:extensions_schema.yaml",
    ],
)

envoy_py_data(
    name = "data",
    src = ":data_srcs",
)

envoy_pytool_binary(
    name = "protodoc",
    srcs = ["protodoc.py"],
    visibility = ["//visibility:public"],
    deps = [
        ":data",
        ":jinja",
        ":validate_py_pb2",
        "//tools/api_proto_plugin",
        "@com_github_cncf_xds//udpa/annotations:pkg_py_proto",
        "@com_github_cncf_xds//xds/annotations/v3:pkg_py_proto",
        requirement("envoy.code.check"),
    ],
)

protodoc_rule(
    name = "api_v3_protodoc",
    visibility = ["//visibility:public"],
    deps = [
        "@envoy_api//:v3_protos",
        "@envoy_api//:xds_protos",
    ],
)

envoy_pytool_library(
    name = "rst_filters",
    srcs = ["rst_filters.py"],
)

envoy_jinja_env(
    name = "jinja",
    env_kwargs = {
        "trim_blocks": True,
        "lstrip_blocks": True,
    },
    filters = {
        "rst_anchor": "tools.protodoc.rst_filters.rst_anchor",
        "rst_header": "tools.protodoc.rst_filters.rst_header",
    },
    templates = [
        "templates/comment.rst.tpl",
        "templates/content.rst.tpl",
        "templates/contrib_message.rst.tpl",
        "templates/empty.rst.tpl",
        "templates/enum.rst.tpl",
        "templates/extension.rst.tpl",
        "templates/extension_category.rst.tpl",
        "templates/file.rst.tpl",
        "templates/header.rst.tpl",
        "templates/message.rst.tpl",
        "templates/security.rst.tpl",
    ],
    deps = [":rst_filters"],
)

cmake_minimum_required(VERSION 3.19)
project(RefactoringToPatterns)

set(CMAKE_CXX_STANDARD 14)

add_executable(RefactoringToPatterns
        adapter.cc
        adapter_before.cc
        builder_composite.cc
        builder_composite_beofre.cc
        command.cc
        command_before.cc
        composite.cc
        composite_before.cc
        composite_extract.cc
        composite_extract_before.cc
        composite_method.cc
        composite_method_before.cc
        creation_factory.cc
        creation_factory_before.cc
        creation_factory_wrapper.cc
        creation_factory_wrapper_before.cc
        creation_method.cc
        decorator.cc
        extract_adapter_before.cc
        factory_method.cc
        factory_method_before.cc
        observer.cc
        observer_before.cc
        singleton.cc
        singleton_before.cc
        state.cc
        state_before.cc
        strategy.cc
        strategy_before.cc
        template_method.cc
        template_method_before.cc)

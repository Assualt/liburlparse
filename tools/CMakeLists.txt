# 生成单元测试覆盖率
add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E echo "Running lcov..."
    COMMAND lcov --ignore-errors unsupported,inconsistent --capture --directory ${CMAKE_SOURCE_DIR} --output-file coverage.info
    COMMAND lcov --ignore-errors unsupported,inconsistent --remove coverage.info '/usr/*' '*/third_party/*' --output-file coverage.info
    COMMAND lcov --ignore-errors unsupported,inconsistent --html coverage.info --output-directory coverage_html
    COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated at coverage_html/index.html"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
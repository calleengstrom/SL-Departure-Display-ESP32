idf_component_register(SRCS "main.cpp"
                        "../utills/wifiSta.cpp"
                        "../utills/httpClient.cpp"
                        "../utills/departureParser.cpp"
                        "../drivers/lcd1602.cpp"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++11)

.PHONY: clean All

All:
	@echo "----------Building project:[ soc_test - BuildSet ]----------"
	@
	@make -f "soc_test.mk" MakeIntermediateDirs && make -f "soc_test.mk" -j 3
clean:
	@echo "----------Cleaning project:[ soc_test - BuildSet ]----------"
	@make -f  "soc_test.mk" clean

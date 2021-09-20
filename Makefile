all:
	g++ -o VariantCaller cvc/Core.cpp cvc/Read.cpp cvc/Reader.cpp cvc/ReadVariant.cpp cvc/Reference.cpp cvc/ReferenceVariant.cpp cvc/main.cpp

clean:
	rm ./VariantCaller
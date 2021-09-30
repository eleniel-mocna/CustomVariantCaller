all:
	g++ -pthread -o VariantCaller.out cvc/Core.cpp cvc/Read.cpp cvc/Reader.cpp cvc/ReadVariant.cpp cvc/Reference.cpp cvc/ReferenceVariant.cpp cvc/Writer.cpp cvc/main.cpp

clean:
	rm ./VariantCaller
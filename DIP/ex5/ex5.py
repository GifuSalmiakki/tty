"""Exercise 5 for Data-Intensive Programming"""

from typing import List, Tuple

from pyspark import SparkContext
from pyspark.sql import SparkSession
from pyspark.rdd import RDD
from operator import add
from pyspark.sql.functions import lower


def main():
    # Create the Spark session
    spark: SparkSession = SparkSession.builder \
                                      .appName("ex5") \
                                      .config("spark.driver.host", "localhost") \
                                      .master("local") \
                                      .getOrCreate()

    # suppress informational log messages related to the inner working of Spark
    sc: SparkContext = spark.sparkContext
    sc.setLogLevel("WARN")

    spark.conf.set("spark.sql.shuffle.partitions", "5")

    # There are three scientific articles in the directory "articles"
    # The call sc.textFile(...) returns an RDD consisting of the lines of the articles:
    articlesRdd: RDD[str] = sc.textFile("articles/*")



    printTaskLine(1)
    # Task #1: How do you get the first 10 lines as a list?
    lines10: List[str] = list(articlesRdd.take(10))
    print(*lines10, sep="\n")



    printTaskLine(2)
    # Task #2: Compute how many lines there are in total in the articles.
    #          And then count the total number of words in the articles
    #          You can assume that words in each line are separated by the space character (i.e. " ")
    nbrOfLines: int = articlesRdd.count()
    print(f"#lines = {nbrOfLines}")

    words: int = articlesRdd.flatMap(lambda row: row.split()).count() # default splits on whitespace
    print(f"#words = {words}")



    printTaskLine(3)
    # Task #3: What is the count of non-white space characters? (it is enough to count the non " "-characters for this)
    #          And how many numerical characters are there in total? (i.e., 0, 1, 2, ..., 9 characters)
    wordRDD = articlesRdd.flatMap(lambda row: row.split())

    chars: int = wordRDD.flatMap(lambda word: [*word]).filter(lambda char: char != " ").count()
    print(f"#chars = {chars}")

    NUMS = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
    numChars: int = wordRDD.flatMap(lambda word: [*word]).filter(lambda char: char in NUMS).count()
    print(f"#numChars = {numChars}")



    printTaskLine(4)
    # Task #4: How many 5-character words that are not "DisCo" are there in the corpus?
    #          And what is the most often appearing 5-character word (that is not "DisCo") and how many times does it appear?
    words5RDD = wordRDD.filter(lambda word: len(word) == 5).filter(lambda word: word != "DisCo")

    words5Count: int = words5RDD.count()
    print(f"5-character words = {words5Count}")

    pair = words5RDD.map(lambda word: (word, 1)).reduceByKey(add).sortBy(lambda x: x[1], ascending=False).first()
    commonWord: str = pair[0]
    commonWordCount: int = pair[1]
    print(f"The most common word is '{commonWord}' and it appears {commonWordCount} times")



    # You are given a factorization function that returns the prime factors for a given number:
    # For example, factorization(28) would return [2, 2, 7]
    def factorization(number: int) -> List[int]:
        def checkFactor(currentNumber: int, factor: int, factorList: List[int]) -> List[int]:
            if currentNumber == 1:
                return factorList
            if factor * factor > currentNumber:
                return factorList + [currentNumber]
            if currentNumber % factor == 0:
                return checkFactor(currentNumber // factor, factor, factorList + [factor])
            return checkFactor(currentNumber, factor + 1, factorList)

        if number < 2:
            return [1]
        return checkFactor(number, 2, [])



    printTaskLine(5)
    # Task #5: You are given a sequence of integers and a factorization function.
    #          Using them create a pair RDD that contains the integers and their prime factors.
    #          Get all the distinct prime factors from the RDD.
    values: List[int] = list(range(12, 18)) + list(range(123, 128)) + list(range(1234, 1238))
    factors = list(map(lambda i: factorization(i), values))

    factorRdd: RDD[Tuple[int, List[int]]] = sc.parallelize(zip(values, factors))
    print(*[f"{n}: {factors}" for n, factors in factorRdd.collect()], sep="\n")

    distinctPrimes: List[int] = []
    print(f"distinct primes: {distinctPrimes}")



    printTaskLine(6)
    # Task #6: Here is a code snippet. Explain how it works.
    lyricsRdd = sc.textFile("lyrics/*.txt")
    # Reads all txt-format lyric files from directory lyrics

    lyricsCount = lyricsRdd.flatMap(lambda line: line.split(" ")) \
                           .map(lambda word: (word, 1)) \
                           .reduceByKey(lambda v1, v2: v1 + v2)
    # First lambda: splits lyric lines into words (words with no whitespace between are counted as one, i.e. "she's")
    # Second lambda: transforms the RDD to (word, count) -pairs, count initialized as one (word appears at least once)
    # Third lambda: finally the lyrics are reduced by combining same words and
    # counting the corresponding values together, so that the final pairs reflect
    # how often the word appears in the lyrics

    print(*(lyricsCount.collect()), sep="\n")



# Helper function to separate the task outputs from each other
def printTaskLine(taskNumber: int) -> None:
    print(f"======\nTask {taskNumber}\n======")


if __name__ == "__main__":
    main()

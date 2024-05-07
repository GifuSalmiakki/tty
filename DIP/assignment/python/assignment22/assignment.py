"""The assignment for Data-Intensive Programming 2022"""

from typing import List, Tuple

from pyspark.sql import DataFrame
from pyspark.sql import SparkSession

from pyspark.sql.types import StructType, StructField, StringType, FloatType, IntegerType, Row
from pyspark.ml.feature import VectorAssembler, MinMaxScaler
from pyspark.ml.clustering import KMeans
from pyspark.ml.pipeline import Pipeline

from pyspark.ml.evaluation import ClusteringEvaluator
from pyspark.sql.dataframe import StorageLevel
import pyspark.sql.functions as f
import matplotlib.pyplot as plt
import numpy

class Assignment:
    spark: SparkSession = SparkSession.builder \
                            .appName("assignment") \
                            .config("spark.driver.host", "localhost") \
                            .master("local") \
                            .getOrCreate()

    # suppress informational log messages related to the inner working of Spark
    spark.sparkContext.setLogLevel("ERROR")
    spark.conf.set("spark.sql.shuffle.partitions", "5")

    # the data frame to be used in tasks 1 and 4

    # Manual schema to interpret the (a, b) points as numbers instead of strings
    schemaD2 = StructType([StructField("a", FloatType(), True),
                           StructField("b", FloatType(), True),
                           StructField("LABEL", StringType(), True)])

    # There's probably a way to add the "c"-column to previous schema but can't figure out the syntax if so
    schemaD3 = StructType([StructField("a", FloatType(), True),
                           StructField("b", FloatType(), True),
                           StructField("c", FloatType(), True),
                           StructField("LABEL", StringType(), True)])

    dataD2: DataFrame = spark.read.option("delimiter", ",").option("header", True).schema(schemaD2) \
        .csv("/home/gifu/tty/DIP/assignment/python/data/dataD2.csv").persist(storageLevel=StorageLevel.MEMORY_ONLY)

    # the data frame to be used in task 2
    dataD3: DataFrame = spark.read.option("delimiter", ",").option("header", True).schema(schemaD3) \
        .csv("/home/gifu/tty/DIP/assignment/python/data/dataD3.csv").persist(storageLevel=StorageLevel.MEMORY_ONLY)

    dataframeForLabels = dataD2
    # the data frame to be used in task 3 (based on dataD2 but containing numeric labels)
    dataD2WithLabels: DataFrame = dataframeForLabels.withColumn("numeric_label", f.when(f.col("LABEL") == "Fatal", 1.0)
                                                                .otherwise(0.0))


    @staticmethod
    def task1(df: DataFrame, k: int) -> List[Tuple[float, float]]:

        COLUMNS = ["a", "b"]
        scaledDF = pipeline(df, COLUMNS)

        kmeans = KMeans(k=k, featuresCol="scaled").setSeed(1)
        kmModel = kmeans.fit(scaledDF)
        centerArray = kmModel.clusterCenters()

        # Reshaping the numpy array to list[tuple]
        centerList = list(map(tuple, centerArray))

        df.unpersist() # clear dataD2 from memory
        return centerList

    @staticmethod
    def task2(df: DataFrame, k: int) -> List[Tuple[float, float, float]]:

        COLUMNS = ["a", "b", "c"]
        scaledDF = pipeline(df, COLUMNS)

        kmeans = KMeans(k=k, featuresCol="scaled").setSeed(1)
        kmModel = kmeans.fit(scaledDF)
        centerArray = kmModel.clusterCenters()

        # Reshaping the numpy array to list[tuple]
        centerList = list(map(tuple, centerArray))

        df.unpersist() # clear dataD3 from memory
        return centerList

    @staticmethod
    def task3(df: DataFrame, k: int) -> List[Tuple[float, float]]:

        COLUMNS = ["a", "b", "numeric_label"]
        scaledDF = pipeline(df, COLUMNS)

        kmeans = KMeans(k=k, featuresCol="scaled").setSeed(1)
        kmModel = kmeans.fit(scaledDF)
        centerArray = kmModel.clusterCenters()

        # sorting descending by numeric label
        sortedArray = sorted(centerArray, key=lambda a: a[2], reverse=True)

        #twoMostFatal = []
        #twoMostFatal.append(sortedArray[0, 0])
        #twoMostFatal.append(sortedArray[(1, 0), (1, 1)])

        # Reshaping the numpy array to list[tuple]
        centerList = list(map(tuple, sortedArray[:2]))

        # returns the label value as well
        # I can't figure out the index syntax to get rid of it
        return centerList



    # Parameter low is the lowest k and high is the highest one.
    @staticmethod
    def task4(df: DataFrame, low: int, high: int) -> List[Tuple[int, float]]:

        silhouettes = []
        kvalues = [*range(low, high+1, 1)]

        COLUMNS = ["a", "b"]
        scaledDF = pipeline(df, COLUMNS)

        for k in kvalues:

            kmeans = KMeans(k=k, featuresCol="scaled").setSeed(1)
            kmModel = kmeans.fit(scaledDF)

            predictions = kmModel.transform(scaledDF)
            evaluator = ClusteringEvaluator()

            silhouette = evaluator.evaluate(predictions)
            print("Silhouette with k-value of "+str(k)+": " + str(silhouette))
            silhouettes.append(silhouette)


        valuePairs = list(zip(kvalues, silhouettes))

        plt.plot(kvalues, silhouettes)
        plt.title("Silhouette scores with different k-values")
        plt.savefig("silhouette-scores-plot.png") # saves to the tests-directory
        plt.show()

        return valuePairs


# Helper function scaling the dataframe
# Param1: raw dataframe
# Param2: columns to scale
# Returns: scaled dataframe
def pipeline(df: DataFrame, cols: list) -> DataFrame:
    vectorAs: VectorAssembler = VectorAssembler(inputCols=cols, outputCol="features")
    mmScaler = MinMaxScaler(inputCol="features", outputCol="scaled", min=0.0, max=1.0)
    pipeline = Pipeline(stages=[vectorAs, mmScaler])

    scalerModel = pipeline.fit(df)
    return scalerModel.transform(df)



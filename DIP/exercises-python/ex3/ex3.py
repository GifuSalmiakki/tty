"""Exercise 3 for Data-Intensive Programming"""

import glob
import pathlib
import shutil
import time
from dataclasses import dataclass

from pyspark.sql import functions
from pyspark.sql import DataFrame
from pyspark.sql import SparkSession


def main():
    # Create the Spark session
    spark: SparkSession = SparkSession.builder \
                                      .appName("ex3") \
                                      .config("spark.driver.host", "localhost") \
                                      .master("local") \
                                      .getOrCreate()

    # suppress log messages related to the inner working of Spark
    spark.sparkContext.setLogLevel("ERROR")

    spark.conf.set("spark.sql.shuffle.partitions", "5")



    printTaskLine(1)
    # Task 1: File "data/sales_data_sample.csv" contains sales data of a retailer.
    #         Study the file and read the data into DataFrame retailerDataFrame.
    #         NOTE: the resulting DataFrame should have 25 columns
    retailerDataFrame: DataFrame = __unknown__



    printTaskLine(2)
    # Task 2: Find the best 10 selling days. That is the days for which QUANTITYORDERED * PRICEEACH
    #         gets the highest values.
    best10DaysDF: DataFrame = __unknown__



    printTaskLine(3)
    # Task 3: This task is originally designed for Scala language. It is not very good for Python
    #         because Python does not support Spark Datasets. Instead of Scala's case class,
    #         you can use Python's dataclass.
    #
    #         The classes that takes a type just like a parameter are known to be Generic
    #         Classes in Scala. Dataset is an example of a generic class. Actually, DataFrame is
    #         a type alias for Dataset[Row], where Row is given as a type parameter. Declare your
    #         own case class (dataclass in Python) Sales with two members: year and euros of type integer.

    #         Then instantiate a DataFrame of Sales and query for the sales on 2019 and
    #         the year of maximum sales.

    salesList = [Sales(2015, 325), Sales(2016, 100), Sales(2017, 15), Sales(2018, 1000),
                 Sales(2019, 50), Sales(2020, 750), Sales(2021, 950), Sales(2022, 400)]
    salesDS = spark.__unknown__

    sales2019 = salesDS.__unknown__
    print(f"Sales for 2019: {sales2019.euros}")

    maximumSales = salesDS.__unknown__
    print(f"Maximum sales: year = {maximumSales.year}, euros = {maximumSales.euros}")



    printTaskLine(4)
    # Task 4: Continuation from task 3.
    #         The new sales list "multiSalesList" contains sales information from multiple sources
    #         and thus can contain multiple values for each year. The total sales in euros for a year
    #         is the sum of all the individual values for that year.
    #         Query for the sales on 2019 and the year with the highest amount of sales in this case.
    multiSalesList = salesList + [Sales(2016, 250), Sales(2017, 600), Sales(2019, 75),
                                  Sales(2020, 225), Sales(2016, 350), Sales(2017, 400)]
    salesDS = spark.__unknown__

    sales2019 = salesDS.__unknown__
    print(f"Total sales for 2019: {sales2019.euros}")

    maximumSales = salesDS.__unknown__
    print(f"Maximum total sales: year = {maximumSales.year}, euros = {maximumSales.euros}")



    printTaskLine(5)
    # Task 5: In the streaming version of the analysis, the streaming data will be added
    #         into the directory streamingData. The streaming data is similar to the one
    #         in the directory "data". It is just divided into multiple files.
    #
    #         Create a DataFrame that will work with streaming data
    #         that is given in the same format as for the static retailerDataFrame.
    #
    #         Note: you cannot really test this task before you have also done the tasks 6 and 7.
    retailerStreamingDF: DataFrame = __unknown__



    printTaskLine(6)
    # Task 6: Find the best selling days in the streaming data
    bestDaysDFStreaming = __unknown__



    printTaskLine(7)
    # Task 7: Test your solution with streaming method by writing the 10 best selling days to stdout
    #         whenever the DataFrame changes

    __unknown__

    # You can test your solution by uncommenting the following code snippet.
    # The loop adds a new CSV file to the directory "streamingData" every 5th second.
    # If you rerun the test remove all the CSV files first from the directory "streamingData".
    # You may need to wait for a while to see the stream processing results while running the program.

    # for file in glob.glob("streamingDataRepo/*"):
    #     shutil.copy(file, f"streamingData/{pathlib.Path(file).name}.csv")
    #     time.sleep(5)


    # Stop the Spark session
    spark.stop()


# Helper function to separate the task outputs from each other
def printTaskLine(taskNumber: int) -> None:
    print(f"======\nTask {taskNumber}\n======")


if __name__ == "__main__":
    main()

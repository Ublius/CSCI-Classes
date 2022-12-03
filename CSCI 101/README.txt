This python program requires two modules not included with the default python install

To install the required modules follow these steps below:

Windows:
	1. Open terminal
	2. Type and enter: "pip install requests"
	3. Type and enter: "pip install beautifulsoup4"

Mac:
	1. Open terminal
	2. Type and enter: "sudo easy_install pip"
	3. Type and enter: "sudo pip install --upgrade pip"
	4. Type and enter: "pip install requests"
	5. Type and enter: "pip install beautifulsoup4"

To use the following program follow these steps below:

How to use:
	1. Enter the URL you would like to scrape html data from
	2. Type the corresponding number associated with the tag you would like to get data from
	3. Repeat step 3 until you have selected all the tags you want
	4. Input nothing to move to next step
	5. Enter the filename name you wish to store all the data to (inlcude .csv at the end)
	6. Browse to csv file to see all the data

Example test case below:

Test Case:

	1. Enter URL> https://www.geeksforgeeks.org/
	2. CHOICE> 13
	3. CHOICE> 14
	4. CHOICE> 19
	5. CHOICE> 
	6. Enter Filename> scrapedData.csv

	Output Data:
			Tag,Content
			p_0,"October 17, 2022"
			p_1,"September 11, 2022"
			p_2,"August 18, 2022"
			p_3,"July 29, 2022"
			p_4,"July 28, 2022"
			h3_0,Improve your Coding Skills with Practice
			h2_1,Start Your Coding Journey Now!

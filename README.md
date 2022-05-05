# Shiva2 DX Cluster for SWL / BCL

Shiva2 DX Cluster connects to a “classic” ham radio dx cluster and repeat the dx spot to shiva2 clients while adding other spots based on .list files and Priyom.org number station schedules.

It is intended to use with SDR Console for SWL/BCL ( see screenshots folder ) so they are able to see broadcasting stations, for example, based on scheduling day and time, spots of ham radio operators, fixed markers about CB or PRM channels, or hf beacons, number stations based on their schedule and so on.
Priyom.org number stations are sent for 1 hour since the start of transmission.

## Block diagram:
see [block.pdf](https://github.com/IU2MEH/shiva2/blob/main/block.pdf)
 

## Getting started
#### If you want to access my shiva2 cluster already running h24, please contact me at https://iu2meh.blogspot.com/
#### I will give you user and password if:
#### - you contribute to this project improving the code 
#### - you contribute in another way like sending me a list or improving this doc or something like
#### - you send me little money [via paypal](https://www.paypal.com/paypalme/iu2meh) to help to cover my server costs

Or you can run your own shiva2 dx cluster:

### Install
Make sure Shiva executable has permission to execute:

chmod +x Shiva2

Create a directory /etc/shiva2/ as root

Download an updated broadcast scheduling list file from http://www.hfcc.org/data/

Actual list is "A20allx2.zip - A20 Operational Schedule - Last updated on 26-May-2020"

Unzip it in /etc/shiva2/ (or any another folder ) and rename the main list ( in our example A20all00.TXT) as LIST.list

You can add other custom list files naming them with suffix .list, see below for the correct format.

Create a user.conf file with users and passwords coma separated, for example:

	iu2meh,thisisthepassword
  
callsign and password MUST be sent immediately after client connection.

### Usage

On 1st run i twill ask you some details:

	A free tcp port to listen for incoming connections
	Your callsign
	Public dx cluster, the “classic” ham radio dx cluster
	Config and .list files directory ( in this case /etc/shiva2 )
	Interval to send all to shiva2 clients
	Timeout
On the subsequent runs it will display your config, 
asking if you want to change them ( default no )

Example:

	./Shiva2
	05052022-11:22:28 Info: Settings found:
	05052022-11:22:28 Info: "TCP port: 4554"
	05052022-11:22:28 Info: "Callsign: iu2meh"
	05052022-11:22:28 Info: "Public DX cluster: sp2put.ampr.org:7300"
	05052022-11:22:28 Info: "Directory with resources: //etc//shiva2////"
	05052022-11:22:28 Info: "Interval: 30 seconds"
	05052022-11:22:28 Info: "Timeout: 4 seconds"
	05052022-11:22:28 Info: Do you want to change them? [y/N]

Possible parameters are:

	-b	runs immediately without ask for settings, useful if you wnt to run shiva2 in background
	-d	enable debug

### Custom list file

You can create a custom list file ( and please share it to me, it will be published here ) creating a new file with suffix .list in the workingdir.

Fields to be specified in the file are the followings, coma separated:

start_date,end_date,day_of_the_week,start_time,end_time,frequency,modulation,station_name

	start_date: day when the station will start to transmit in the format ddmmyyyy
	end_date: day when the station will stop to tx in the format ddmmyyyy, 31122100 is taken as standard for forever.
	day_of_the_week: day of the week when the station is active, 1 for monday 7 for sunday, for example if you write 123467 means that is active from monday to sunday except friday
	start_time:
	end_time: start and end time in UTC of tx, in the format hhmm, if the station transmits h24 you have to write 0000,2359 . Do NOT use 2400 as midnight
	frequency: self explanatory, in Hertz
	modulation: AM or LSB or USB or whatever you want
	station_name: self explanatory

Example:

28052020,31122100,1234567,0000,2359,252000,AM,Chaine 3 (AL)

means: Chaine 3 from Algeria is transmitting on 252kHz in AM 24 hours a day everyday. (year 2100 means forever)

If you want to add an yearly event you can use the following notation:

17062020,1706****,1234567,0000,2359,999000,AM,test

means: station "test" will transmit on 999kHz AM for 24 hours every 17th of June.

#### I want to repeat... please SHARE your list files with this project and their users sending it to me.

## Authors

* **Walter Panella IU2MEH** - *Initial work*

## Known Bugs
--


## ToDo:

- Connect to reverse beacon network
- Connect to multiple "classic" dx cluster and merge spots

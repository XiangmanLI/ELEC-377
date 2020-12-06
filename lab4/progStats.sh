#/bin/sh test2out.txt


if [[ "$#" -ne 1 ]];then
	echo "Usage:" $0 `pwd`
else
	if [[ -d "$1" ]];then

		#checking Main Files
		echo "Main files:"

		#set the flag to false
		MainFile=false
		#get .c files in full directory
		for f in $(find $1 -type f -name "*.c"); do
			#if the line mumber smaller than 0, there is no int main in .c files 
			if((`grep -c "int main" $f` > 0)); then
				#set the flag to true
				MainFile=true

				#get the number of fprintf and printf
				fprinf_Count=`grep -c 'fprintf' $f`
				prinf=`grep -c 'printf' $f`
				prinf_Count=`expr $prinf - $fprinf_Count`
				
				echo $f,$prinf_Count,$fprinf_Count | awk -F '[ ,]'  '{print $1,$2,$3}'
			fi
		done

		echo $MainFile

		if [[ $MainFile == "false" ]]; then
			echo "main file not found !!!!!"
		fi
		#checking Modules Files
		echo "Modules files:"

		ModulesFile=false
		for f in $(find $1 -type f -name "*.c"); do
			if((`grep -c "init_module" $f` > 0)); then
				ModulesFile=true

				#get the line of "printk", and delete the content of printk, then use tr to change the space to ,
				printK=`grep -n "printk" $f | sed -e 's/:.*//' | tr '\n' ',' | sed -e 's/,$//'`
	
				echo $f : $printK
			fi
		done

		if [[ $ModulesFile == "false" ]]; then
			echo "modules file not found !!!!!"
		fi
	else
		echo "ERROR: path is not correct!"
	fi
fi


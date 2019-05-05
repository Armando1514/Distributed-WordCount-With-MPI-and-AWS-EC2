# Tutorial
This tutorial refers to a system with a Linux OS (in this case Ubuntu)
## Prerequisites
**0. AWS Account** (in my case is a student account)\
## Requirements for start to work with AWS:
**1.  Amazon CLI**\
the installation is easy, just open the terminal and write:
  ```bash
    curl ”https://bootstrap.pypa.io/get-pip.py” -o ”get-pip.py”
    sudo python get-pip.py
    sudo pip install awscli
  ``` 
After that, you need to configure your CLI, go in your account, click on "Account Detalis", like
the screen below:
![screen of AWS account](screenshot1.jpg)
click on "Show", near AWS CLI and copy all the code starting from [default] and paste in "~/.aws/credentials".
Well Done! The first step is over.\
**2.  Create a Security Group**\
Why? Security group are virtual firewall for instance EC2 that control inbound and outbound traffic(who is able to use your ec2 instance), in our example we don't care about security, for that reason i give the max permission. Write this code in the terminal:
  ``` bash
     aws ec2 create-security-group --group-name noSecurity --description "security group for example all the traffic is allowed"
     aws ec2 authorize-security-group-ingress --group-name noSecurity --protocol -1 --port 0-65535 --cidr 0.0.0.0/0
```
Remember to save the id of the security group that return the execution of the first line, because we need it for the step 4. (If you forget, is possibile to see the id in the Amazon console web based, go in the section EC2 and select in the menù the field "Security groups")
Another step is over!\
**3.  Create and download SSH credential**\
Why? It's a secure way to remotely access a EC2 instance, write:
  ```bash
     aws ec2 create-key-pair --key-name access-key --query 'KeyMaterial' --output text > access-key.pem
     chmod 400 access-key.pem
```
Done!\
**4.  Run an EC2 instance of Ubuntu Server 16.04 LTS**\
**Replace --security-group-id exampleID with your security group id (step 2).**
```bash
     aws ec2 run-instances --image-id ami-f4cc1de2 --security-group-ids exampleID --count 1 --instance-type t2.micro --key-name access-key --query 'Instances[0].InstanceId'
```
Now the output is the id of the instances, replace "IstanceID" with the output

 ```bash
     aws ec2 describe-instances --instance-ids InstanceID --query 'Reservations[0].Instances[0].PublicIpAddress'
```
Remember the output is the public ip of the EC2 instance.\
**5. Connect to the instance by SSH**\
Replace ipOutput with the previous output and write:
 ```bash
     ssh -i access-key.pem ubuntu@ipOutput
```
Now you connected your terminal to the EC2 instance with Ubuntu OS. (You are able to use Amazon web console for make the same steps through browser, and also see the instance description, like ip, security groups etc.)


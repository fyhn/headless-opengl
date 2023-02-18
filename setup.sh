# Install packages
sudo yum install -y gcc-c++
sudo yum install -y mesa-libEGL-devel
sudo yum install -y libpng

# Install and setup docker
sudo yum install -y docker
sudo usermod -a -G docker ec2-user
id ec2-user
newgrp docker

# Start docker service
sudo systemctl enable docker.service
sudo systemctl start docker.service

# Run an nginx container
# TODO Add a check if it's already running, in case this script is run twice by mistake
docker run -d --rm -p 80:80 --restart unless-stopped --name nginx -v $PWD/html:/usr/share/nginx/html:ro -v $PWD/default.conf:/etc/nginx/conf.d/default.conf nginx

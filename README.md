docker build -t debianssh .

docker run -d -p 8022:22 debianssh 
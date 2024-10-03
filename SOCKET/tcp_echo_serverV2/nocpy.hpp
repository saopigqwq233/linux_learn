class nocpy{
public:
    nocpy(){}
    nocpy(const nocpy&)=delete;
    const nocpy& operator=(const nocpy&)=delete;
    ~nocpy(){}    
};
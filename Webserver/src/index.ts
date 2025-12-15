const express = require("express");

const app = express();
const PORT = 2777;

app.get("/",(_req: any, res: any)=>{
    res.send("TEST Server");
});

app.listen(PORT, ()=>{
    console.log(`Server running on localhost @${PORT}`);
})
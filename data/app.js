
let ws; const cv=document.getElementById('preview'); const ctx=cv.getContext('2d');
const scale=Math.min(window.innerWidth*0.9/128,6); cv.style.width=`${128*scale}px`; cv.style.imageRendering='pixelated';
const state={x:0,y:20,text:'',color:'#ffffff'};
function currentPt(){const el=document.querySelector('input[name="pt"]:checked');return parseInt(el.value);} 
function renderPreview(){ctx.fillStyle='#000';ctx.fillRect(0,0,128,64);ctx.fillStyle=state.color;ctx.font=`${currentPt()}px Noto Sans KR, sans-serif`;ctx.textBaseline='alphabetic';ctx.fillText(state.text,state.x,state.y);} 
let dragging=false; function updatePos(e){const rect=cv.getBoundingClientRect();const px=Math.floor((e.clientX-rect.left)/scale);const py=Math.floor((e.clientY-rect.top)/scale);state.x=Math.max(0,Math.min(127,px));state.y=Math.max(0,Math.min(63,py));renderPreview();}
cv.addEventListener('pointerdown',e=>{dragging=true;updatePos(e);}); cv.addEventListener('pointermove',e=>{if(dragging)updatePos(e);}); cv.addEventListener('pointerup',()=>dragging=false); cv.addEventListener('pointerleave',()=>dragging=false);
window.addEventListener('load',()=>{ws=new WebSocket(`ws://${location.host}/ws`);
 document.getElementById('sendTxt').onclick=()=>{state.text=document.getElementById('txt').value; state.color=document.getElementById('color').value; renderPreview(); const payload={text:state.text,x:state.x,y:state.y,pt:currentPt(),color:state.color}; ws.send(JSON.stringify(payload));};
 document.getElementById('upload').onclick=()=>{const f=document.getElementById('file').files[0]; if(!f)return alert('파일을 선택하세요'); const fd=new FormData(); fd.append('img',f,f.name); const xhr=new XMLHttpRequest(); xhr.upload.onprogress=(e)=>{if(e.lengthComputable){const p=Math.round(e.loaded/e.total*100); document.getElementById('bar').style.width=p+'%';}}; xhr.onload=()=>alert('업로드 완료'); xhr.onerror=()=>alert('업로드 실패'); xhr.open('POST','/upload'); xhr.send(fd);};
 renderPreview();});

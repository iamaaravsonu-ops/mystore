document.addEventListener('DOMContentLoaded', () => {
    // Open Calculator Modal inside header
    const btnOpenCalc = document.getElementById('openCalc');
    const calcModal = document.getElementById('calcModal');

    if(btnOpenCalc && calcModal) {
        btnOpenCalc.addEventListener('click', () => {
            calcModal.classList.add('active');
            document.body.style.overflow = 'hidden';
        });
    }

    // Tabs inside calc modal
    const calcTabs = document.querySelectorAll('.calc-tab');
    calcTabs.forEach(btn => {
        btn.addEventListener('click', (e) => {
            calcTabs.forEach(b => b.classList.remove('active'));
            document.querySelectorAll('.calc-view').forEach(c => c.classList.remove('active'));
            e.target.classList.add('active');
            document.getElementById(e.target.dataset.target).classList.add('active');
        });
    });

    // ==========================================
    // CARPET CALCULATOR LOGIC
    // EXACT formula requested by user
    // ==========================================
    const FT_TO_M = 0.3048;

    function calculateCarpet(roomLengthFt, roomWidthFt, carpetWidthFt) {
        const lenM  = roomLengthFt  * FT_TO_M;
        const widM  = roomWidthFt   * FT_TO_M;
        const carpM = carpetWidthFt * FT_TO_M;
        const strips = Math.ceil(widM / carpM);  // always round UP
        const totalMeters = strips * lenM;
        return totalMeters.toFixed(2);
    }

    const btnCalc = document.getElementById('btnCalc');
    if(btnCalc) {
        btnCalc.addEventListener('click', () => {
            const rL = parseFloat(document.getElementById('rLength').value);
            const rW = parseFloat(document.getElementById('rWidth').value);
            const cW = parseFloat(document.getElementById('cWidth').value);
            const err = document.getElementById('calcError');
            const resBox = document.getElementById('calcResult');
            const resMeters = document.getElementById('resMeters');

            if (isNaN(rL) || isNaN(rW) || isNaN(cW) || rL <= 0 || rW <= 0 || cW <= 0) {
                err.style.display = 'block';
                resBox.style.display = 'none';
                return;
            }
            
            err.style.display = 'none';
            resMeters.textContent = calculateCarpet(rL, rW, cW);
            resBox.style.display = 'block';
        });
    }

    // ==========================================
    // CURRENCY CONVERTER LOGIC
    // 1 NPR = 0.625 INR
    // 1 INR = 1.60 NPR
    // ==========================================
    const nprVal = document.getElementById('nprVal');
    const inrVal = document.getElementById('inrVal');
    const btnSwap = document.getElementById('btnSwap');

    const RATE_NPR_TO_INR = 0.625;
    const RATE_INR_TO_NPR = 1.60;

    if(nprVal && inrVal) {
        nprVal.addEventListener('input', () => {
            const val = parseFloat(nprVal.value);
            if(!isNaN(val)) {
                inrVal.value = (val * RATE_NPR_TO_INR).toFixed(2);
            } else {
                inrVal.value = '';
            }
        });

        inrVal.addEventListener('input', () => {
            const val = parseFloat(inrVal.value);
            if(!isNaN(val)) {
                nprVal.value = (val * RATE_INR_TO_NPR).toFixed(2);
            } else {
                nprVal.value = '';
            }
        });

        // Swap function
        btnSwap.addEventListener('click', () => {
            const temp = nprVal.value;
            nprVal.value = inrVal.value;
            inrVal.value = temp;

            // Trigger conversion after swapping based on highest priority (npr if has value, else inr)
            if(nprVal.value) {
                inrVal.value = (parseFloat(nprVal.value) * RATE_NPR_TO_INR).toFixed(2);
            } else if (inrVal.value) {
                nprVal.value = (parseFloat(inrVal.value) * RATE_INR_TO_NPR).toFixed(2);
            }
        });
    }

});
